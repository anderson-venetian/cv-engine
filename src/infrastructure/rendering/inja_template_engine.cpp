#include <cvengine/infrastructure/rendering/inja_template_engine.hpp>
#include <cvengine/infrastructure/rendering/latex_escape.hpp>

#include <fstream>
#include <sstream>
#include <vector>
#include <inja/inja.hpp>
#include <nlohmann/json.hpp>

namespace cvengine::infrastructure::rendering {

namespace {

using json = nlohmann::json;
namespace dom = core::domain;
using common::ErrorCode;
using common::make_error;

// Convierte una clave dotted "typography.base_size" en escritura jerárquica
// dentro de un JSON object: target.typography.base_size = value.
// Las partes intermedias se crean como objects si no existen.
auto set_nested(json& target, const std::string& dotted_key, const std::string& value) -> void {
    std::vector<std::string> parts;
    std::stringstream ss{dotted_key};
    std::string part;
    while (std::getline(ss, part, '.')) {
        if (!part.empty()) parts.push_back(part);
    }
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

    ctx["personal"] = {
        {"name",     escape_latex(cv.contact().name())},
        {"location", escape_latex(cv.contact().location())},
        {"phone",    escape_latex(cv.contact().phone())},
        {"email",    escape_latex(cv.contact().email())}
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

// El Theme guarda claves dotted ("typography.base_size") por elección
// arquitectónica del dominio. Aquí las desplaño a estructura anidada
// para que inja las acceda como theme.properties.typography.base_size.
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
        std::ostringstream msg;
        msg << "Template not found: " << template_path.string();
        return make_error(ErrorCode::FileNotFound, msg.str());
    }

    std::ifstream file{template_path};
    if (!file) {
        std::ostringstream msg;
        msg << "Cannot open template: " << template_path.string();
        return make_error(ErrorCode::IoError, msg.str());
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
        std::ostringstream msg;
        msg << "Inja render error: " << e.what();
        return make_error(ErrorCode::RenderError, msg.str());
    }
}

} // namespace cvengine::infrastructure::rendering
