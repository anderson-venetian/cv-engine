#include <cvengine/infrastructure/persistence/json_data_repository.hpp>
#include <cvengine/common/validation.hpp>

#include <fstream>
#include <sstream>
#include <vector>
#include <nlohmann/json.hpp>

namespace cvengine::infrastructure::persistence {

namespace {

using json = nlohmann::json;
namespace dom = core::domain;
using common::ErrorCode;
using common::make_error;

auto get_string(const json& obj, const char* key) -> common::Result<std::string> {
    if (!obj.contains(key)) {
        std::ostringstream msg;
        msg << "Missing required field: " << key;
        return make_error(ErrorCode::EmptyRequiredField, msg.str());
    }
    if (!obj[key].is_string()) {
        std::ostringstream msg;
        msg << "Field '" << key << "' must be a string";
        return make_error(ErrorCode::InvalidFormat, msg.str());
    }
    return obj[key].get<std::string>();
}

auto get_string_or(const json& obj, const char* key, std::string_view fallback) -> std::string {
    if (obj.contains(key) && obj[key].is_string()) {
        return obj[key].get<std::string>();
    }
    return std::string{fallback};
}

auto build_extra_channels(const json& personal) -> common::Result<std::vector<dom::ContactChannel>> {
    std::vector<dom::ContactChannel> result;

    if (!personal.contains("channels") || !personal["channels"].is_array()) {
        return result;
    }

    for (const auto& ch_json : personal["channels"]) {
        auto type_str = get_string(ch_json, "type");
        auto label    = get_string(ch_json, "label");
        auto value    = get_string(ch_json, "value");
        if (!type_str) return std::unexpected{type_str.error()};
        if (!label)    return std::unexpected{label.error()};
        if (!value)    return std::unexpected{value.error()};

        auto type = dom::ContactChannel::type_from_string(*type_str);
        auto ch = dom::ContactChannel::create(type, *label, *value);
        if (!ch) return std::unexpected{ch.error()};
        result.push_back(*std::move(ch));
    }
    return result;
}

auto build_contact(const json& j) -> common::Result<dom::Contact> {
    auto& personal = j["personal"];
    auto name     = get_string(personal, "name");
    auto location = get_string(personal, "location");
    auto email    = get_string(personal, "email");
    if (!name)     return std::unexpected{name.error()};
    if (!location) return std::unexpected{location.error()};
    if (!email)    return std::unexpected{email.error()};

    auto phone = get_string_or(personal, "phone", "");

    auto channels = build_extra_channels(personal);
    if (!channels) return std::unexpected{channels.error()};

    return dom::Contact::create(*name, *location, phone, *email, *std::move(channels));
}

auto build_profile(const json& j) -> common::Result<dom::ProfessionalProfile> {
    auto text = get_string(j, "profile");
    if (!text) return std::unexpected{text.error()};
    return dom::ProfessionalProfile::create(*text);
}

auto build_skills(const json& j) -> common::Result<std::vector<dom::SkillGroup>> {
    std::vector<dom::SkillGroup> result;
    if (!j.contains("skills") || !j["skills"].is_object()) {
        return result;
    }
    for (auto it = j["skills"].begin(); it != j["skills"].end(); ++it) {
        if (!it.value().is_string()) continue;
        auto sg = dom::SkillGroup::create(it.key(), it.value().get<std::string>());
        if (!sg) return std::unexpected{sg.error()};
        result.push_back(*std::move(sg));
    }
    return result;
}

auto build_experience(const json& exp_json) -> common::Result<dom::Experience> {
    auto position     = get_string(exp_json, "position");
    auto organization = get_string(exp_json, "organization");
    auto location     = get_string(exp_json, "location");
    auto period_text  = get_string(exp_json, "period");
    if (!position)     return std::unexpected{position.error()};
    if (!organization) return std::unexpected{organization.error()};
    if (!location)     return std::unexpected{location.error()};
    if (!period_text)  return std::unexpected{period_text.error()};

    auto period = dom::Period::create(*period_text);
    if (!period) return std::unexpected{period.error()};

    if (!exp_json.contains("achievements") || !exp_json["achievements"].is_array()) {
        return make_error(ErrorCode::EmptyRequiredField,
            "Experience: achievements array required");
    }

    std::vector<dom::Achievement> achievements;
    for (const auto& ach_json : exp_json["achievements"]) {
        if (!ach_json.is_string()) continue;
        auto ach = dom::Achievement::create(ach_json.get<std::string>());
        if (!ach) return std::unexpected{ach.error()};
        achievements.push_back(*std::move(ach));
    }

    return dom::Experience::create(
        *position, *organization, *location,
        *std::move(period), std::move(achievements)
    );
}

auto build_experiences(const json& j) -> common::Result<std::vector<dom::Experience>> {
    std::vector<dom::Experience> result;
    if (!j.contains("experiences") || !j["experiences"].is_array()) {
        return make_error(ErrorCode::EmptyRequiredField,
            "JSON: 'experiences' array required");
    }
    for (const auto& exp_json : j["experiences"]) {
        auto exp = build_experience(exp_json);
        if (!exp) return std::unexpected{exp.error()};
        result.push_back(*std::move(exp));
    }
    return result;
}

auto build_education_entry(const json& edu_json) -> common::Result<dom::Education> {
    auto degree      = get_string(edu_json, "degree");
    auto institution = get_string(edu_json, "institution");
    auto date        = get_string(edu_json, "completion_date");
    if (!degree)      return std::unexpected{degree.error()};
    if (!institution) return std::unexpected{institution.error()};
    if (!date)        return std::unexpected{date.error()};

    auto location = get_string_or(edu_json, "location", "");
    return dom::Education::create(*degree, *institution, location, *date);
}

auto build_education(const json& j) -> common::Result<std::vector<dom::Education>> {
    std::vector<dom::Education> result;
    if (!j.contains("education") || !j["education"].is_array()) {
        return make_error(ErrorCode::EmptyRequiredField,
            "JSON: 'education' array required");
    }
    for (const auto& edu_json : j["education"]) {
        auto edu = build_education_entry(edu_json);
        if (!edu) return std::unexpected{edu.error()};
        result.push_back(*std::move(edu));
    }
    return result;
}

auto build_certifications(const json& j) -> common::Result<std::vector<dom::Certification>> {
    std::vector<dom::Certification> result;
    if (!j.contains("certifications") || !j["certifications"].is_array()) {
        return result;
    }
    for (const auto& cert_json : j["certifications"]) {
        auto name   = get_string(cert_json, "name");
        auto issuer = get_string(cert_json, "issuer");
        auto date   = get_string(cert_json, "date");
        if (!name)   return std::unexpected{name.error()};
        if (!issuer) return std::unexpected{issuer.error()};
        if (!date)   return std::unexpected{date.error()};

        auto cert = dom::Certification::create(*name, *issuer, *date);
        if (!cert) return std::unexpected{cert.error()};
        result.push_back(*std::move(cert));
    }
    return result;
}

} // anonymous namespace

JsonDataRepository::JsonDataRepository(std::filesystem::path base_directory)
    : base_directory_{std::move(base_directory)} {}

auto JsonDataRepository::load(std::string_view identifier) const
    -> common::Result<dom::Cv> {

    // Reject identifiers containing path separators or shell metacharacters
    // to prevent path traversal (e.g. "../../etc/passwd").
    if (!common::is_safe_identifier(identifier)) {
        return make_error(ErrorCode::InvalidFormat,
            "Data identifier contains invalid characters "
            "(only alphanumeric, hyphens, underscores, dots allowed)");
    }

    auto path = base_directory_ / (std::string{identifier} + ".json");

    if (!std::filesystem::exists(path)) {
        std::ostringstream msg;
        msg << "File not found: " << path.string();
        return make_error(ErrorCode::FileNotFound, msg.str());
    }

    std::ifstream file{path};
    if (!file) {
        std::ostringstream msg;
        msg << "Cannot open file: " << path.string();
        return make_error(ErrorCode::IoError, msg.str());
    }

    json j;
    try {
        file >> j;
    } catch (const json::parse_error& e) {
        std::ostringstream msg;
        msg << "JSON parse error: " << e.what();
        return make_error(ErrorCode::ParseError, msg.str());
    }

    auto contact        = build_contact(j);
    if (!contact)        return std::unexpected{contact.error()};

    auto profile        = build_profile(j);
    if (!profile)        return std::unexpected{profile.error()};

    auto skills         = build_skills(j);
    if (!skills)         return std::unexpected{skills.error()};

    auto experiences    = build_experiences(j);
    if (!experiences)    return std::unexpected{experiences.error()};

    auto education      = build_education(j);
    if (!education)      return std::unexpected{education.error()};

    auto certifications = build_certifications(j);
    if (!certifications) return std::unexpected{certifications.error()};

    auto language = get_string_or(j, "language", "es");

    return dom::Cv::create(
        *std::move(contact),
        *std::move(profile),
        *std::move(skills),
        *std::move(experiences),
        *std::move(education),
        *std::move(certifications),
        language
    );
}

} // namespace cvengine::infrastructure::persistence
