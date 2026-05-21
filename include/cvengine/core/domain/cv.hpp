#pragma once

#include <string>
#include <vector>
#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/contact.hpp>
#include <cvengine/core/domain/professional_profile.hpp>
#include <cvengine/core/domain/skill_group.hpp>
#include <cvengine/core/domain/experience.hpp>
#include <cvengine/core/domain/education.hpp>
#include <cvengine/core/domain/certification.hpp>

namespace cvengine::core::domain {

// Agregado raíz: representa un CV completo y consistente.
// La construcción solo es posible vía Cv::create, que valida invariantes.
class Cv {
public:
    [[nodiscard]] static auto create(
        Contact contact,
        ProfessionalProfile profile,
        std::vector<SkillGroup> skills,
        std::vector<Experience> experiences,
        std::vector<Education> education,
        std::vector<Certification> certifications,
        std::string language = "es"
    ) -> common::Result<Cv>;

    [[nodiscard]] auto contact()        const noexcept -> const Contact&             { return contact_; }
    [[nodiscard]] auto profile()        const noexcept -> const ProfessionalProfile& { return profile_; }
    [[nodiscard]] auto skills()         const noexcept -> const std::vector<SkillGroup>&    { return skills_; }
    [[nodiscard]] auto experiences()    const noexcept -> const std::vector<Experience>&    { return experiences_; }
    [[nodiscard]] auto education()      const noexcept -> const std::vector<Education>&     { return education_; }
    [[nodiscard]] auto certifications() const noexcept -> const std::vector<Certification>& { return certifications_; }
    [[nodiscard]] auto language()       const noexcept -> const std::string&         { return language_; }

private:
    Cv(
        Contact contact,
        ProfessionalProfile profile,
        std::vector<SkillGroup> skills,
        std::vector<Experience> experiences,
        std::vector<Education> education,
        std::vector<Certification> certifications,
        std::string language
    )
        : contact_{std::move(contact)}
        , profile_{std::move(profile)}
        , skills_{std::move(skills)}
        , experiences_{std::move(experiences)}
        , education_{std::move(education)}
        , certifications_{std::move(certifications)}
        , language_{std::move(language)} {}

    Contact contact_;
    ProfessionalProfile profile_;
    std::vector<SkillGroup> skills_;
    std::vector<Experience> experiences_;
    std::vector<Education> education_;
    std::vector<Certification> certifications_;
    std::string language_;
};

} // namespace cvengine::core::domain
