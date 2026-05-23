#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/cv.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Cv aggregate root validations", "[core][domain]") {
    using cvengine::core::domain::Cv;
    using cvengine::core::domain::Contact;
    using cvengine::core::domain::ProfessionalProfile;
    using cvengine::core::domain::SkillGroup;
    using cvengine::core::domain::Experience;
    using cvengine::core::domain::Education;
    using cvengine::core::domain::Certification;
    using cvengine::core::domain::Period;
    using cvengine::core::domain::Achievement;
    using cvengine::common::ErrorCode;

    // Crear entidades válidas requeridas
    auto contact_r = Contact::create("Pedro Espinoza", "Lima", "123", "pedro@test.com");
    REQUIRE(contact_r.has_value());

    auto profile_r = ProfessionalProfile::create("Tecnico en Computacion e Informatica especializado en Service Desk y Help Desk N1/N2 con experiencia en gestion de tickets, SLA y CSAT.");
    REQUIRE(profile_r.has_value());

    auto period_r = Period::create("2020-2021");
    REQUIRE(period_r.has_value());

    std::vector<Achievement> achievements { Achievement::create("Logro 1").value() };
    auto exp_r = Experience::create("Cargo", "Empresa", "Lima", *period_r, achievements);
    REQUIRE(exp_r.has_value());

    auto edu_r = Education::create("Técnico", "IES", "Lima", "2020");
    REQUIRE(edu_r.has_value());

    SECTION("Valid CV creation") {
        std::vector<SkillGroup> skills;
        std::vector<Experience> experiences { *exp_r };
        std::vector<Education> education { *edu_r };
        std::vector<Certification> certifications;

        auto cv_r = Cv::create(
            *contact_r,
            *profile_r,
            skills,
            experiences,
            education,
            certifications,
            "es"
        );
        REQUIRE(cv_r.has_value());
        CHECK(cv_r->language() == "es");
    }

    SECTION("Fail on empty experiences") {
        std::vector<SkillGroup> skills;
        std::vector<Experience> experiences; // empty
        std::vector<Education> education { *edu_r };
        std::vector<Certification> certifications;

        auto cv_r = Cv::create(
            *contact_r,
            *profile_r,
            skills,
            experiences,
            education,
            certifications,
            "es"
        );
        REQUIRE_FALSE(cv_r.has_value());
        CHECK(cv_r.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty education") {
        std::vector<SkillGroup> skills;
        std::vector<Experience> experiences { *exp_r };
        std::vector<Education> education; // empty
        std::vector<Certification> certifications;

        auto cv_r = Cv::create(
            *contact_r,
            *profile_r,
            skills,
            experiences,
            education,
            certifications,
            "es"
        );
        REQUIRE_FALSE(cv_r.has_value());
        CHECK(cv_r.error().code == ErrorCode::EmptyRequiredField);
    }
}
