#include <cvengine/infrastructure/rendering/inja_template_engine.hpp>
#include <cvengine/infrastructure/rendering/latex_escape.hpp>
#include <cvengine/common/string_utils.hpp>

#include <fstream>
#include <sstream>
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

namespace cvengine::infrastructure::rendering {

namespace {

using json = nlohmann::json;
namespace dom = core::domain;
using common::ErrorCode;
using common::make_error;

auto set_nested(json& target, const std::string& dotted_key, const std::string& value) -> void {
    auto parts = common::split(dotted_key, '.');
    if (parts.empty()) return;

    json* current = &target;
    for (std::size_t i = 0; i + 1 < parts.size(); ++i) {
        if (!current->contains(parts[i]) || !(*current)[parts[i]].is_object()) {
            (*current)[parts[i]] = json::object();
        }
        current = &(*current)[parts[i]];
    }
    (*current)[parts.back()] = value;
}

auto cv_to_context(const dom::Cv& cv) -> json {
    json ctx;

    // Canales extra de contacto (URL ya construida en el dominio).
    // La URL NO se escapa porque LaTeX la usa como argumento de \href,
    // no como texto visible. El value sí se escapa (es texto visible).
    json channels_arr = json::array();
    for (const auto& ch : cv.contact().extra_channels()) {
        channels_arr.push_back({
            {"type",  std::string{dom::ContactChannel::type_to_string(ch.type())}},
            {"label", escape_latex(ch.label())},
            {"value", escape_latex(ch.value())},
            {"url",   ch.url()}
        });
    }

    ctx["personal"] = {
        {"name",     escape_latex(cv.contact().name())},
        {"location", escape_latex(cv.contact().location())},
        {"phone",    escape_latex(cv.contact().phone())},
        {"email",    escape_latex(cv.contact().email())},
        {"email_raw", cv.contact().email()},
        {"channels", std::move(channels_arr)}
    };

    ctx["profile"] = escape_latex(cv.profile().text());

    json skills_arr = json::array();
    for (const auto& s : cv.skills()) {
        skills_arr.push_back({
            {"label",   escape_latex(s.label())},
            {"content", escape_latex(s.content())}
        });
    }
    ctx["skills"] = std::move(skills_arr);

    json experiences_arr = json::array();
    for (const auto& exp : cv.experiences()) {
        json achievements_arr = json::array();
        for (const auto& ach : exp.achievements()) {
            achievements_arr.push_back(escape_latex(ach.text()));
        }
        experiences_arr.push_back({
            {"position",     escape_latex(exp.position())},
            {"organization", escape_latex(exp.organization())},
            {"location",     escape_latex(exp.location())},
            {"period",       escape_latex(exp.period().value())},
            {"achievements", std::move(achievements_arr)}
        });
    }
    ctx["experiences"] = std::move(experiences_arr);

    json education_arr = json::array();
    for (const auto& edu : cv.education()) {
        education_arr.push_back({
            {"degree",          escape_latex(edu.degree())},
            {"institution",     escape_latex(edu.institution())},
            {"location",        escape_latex(edu.location())},
            {"completion_date", escape_latex(edu.completion_date())}
        });
    }
    ctx["education"] = std::move(education_arr);

    json certs_arr = json::array();
    for (const auto& c : cv.certifications()) {
        certs_arr.push_back({
            {"name",   escape_latex(c.name())},
            {"issuer", escape_latex(c.issuer())},
            {"date",   escape_latex(c.date())}
        });
    }
    ctx["certifications"] = std::move(certs_arr);

    ctx["language"] = cv.language();
    return ctx;
}

auto theme_to_context(const dom::Theme& theme) -> json {
    json ctx;
    ctx["name"] = theme.name();

    json props = json::object();
    for (const auto& [key, value] : theme.properties()) {
        set_nested(props, key, value);
    }
    ctx["properties"] = std::move(props);
    return ctx;
}

} // anonymous namespace

InjaTemplateEngine::InjaTemplateEngine(std::filesystem::path templates_directory)
    : templates_directory_{std::move(templates_directory)} {}

auto InjaTemplateEngine::render(
    std::string_view template_name,
    const dom::Cv& cv,
    const dom::Theme& theme
) const -> common::Result<std::string> {

    auto template_path = templates_directory_ / std::string{template_name};

    if (!std::filesystem::exists(template_path)) {
        return make_error(ErrorCode::FileNotFound,
            common::concat("Template not found: ", template_path.string()));
    }

    std::ifstream file{template_path};
    if (!file) {
        return make_error(ErrorCode::IoError,
            common::concat("Cannot open template: ", template_path.string()));
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string template_content = buffer.str();

    json context;
    context["cv"]    = cv_to_context(cv);
    context["theme"] = theme_to_context(theme);

    inja::Environment env;
    env.set_expression("((", "))");
    env.set_statement("##", "##");
    env.set_comment("(#", "#)");
    env.set_trim_blocks(true);
    env.set_lstrip_blocks(true);

    try {
        return env.render(template_content, context);
    } catch (const inja::InjaError& e) {
        return make_error(ErrorCode::RenderError,
            common::concat("Inja render error: ", e.what()));
    }
}

} // namespace cvengine::infrastructure::rendering
