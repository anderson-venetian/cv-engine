#include <cvengine/core/domain/cv.hpp>

namespace cvengine::core::domain {

auto Cv::create(
    Contact contact,
    ProfessionalProfile profile,
    std::vector<SkillGroup> skills,
    std::vector<Experience> experiences,
    std::vector<Education> education,
    std::vector<Certification> certifications,
    std::string language
) -> common::Result<Cv> {
    if (experiences.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Cv requires at least one experience"
        );
    }
    if (education.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Cv requires at least one education entry"
        );
    }
    if (language.empty()) {
        language = "es";
    }

    return Cv{
        std::move(contact),
        std::move(profile),
        std::move(skills),
        std::move(experiences),
        std::move(education),
        std::move(certifications),
        std::move(language)
    };
}

} // namespace cvengine::core::domain
