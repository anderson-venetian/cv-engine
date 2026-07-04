#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/professional_profile.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("ProfessionalProfile domain entity validations", "[core][domain]") {
    using cvengine::core::domain::ProfessionalProfile;
    using cvengine::common::ErrorCode;

    const std::string valid_text =
        "Tecnico en Computacion e Informatica especializado en Service Desk "
        "y Help Desk N1/N2 con experiencia en gestion de tickets.";

    SECTION("Valid profile creation") {
        auto result = ProfessionalProfile::create(valid_text);
        REQUIRE(result.has_value());
        CHECK(result->text() == valid_text);
    }

    SECTION("Trim whitespace") {
        auto padded = "  " + valid_text + "  ";
        auto result = ProfessionalProfile::create(padded);
        REQUIRE(result.has_value());
        CHECK(result->text() == valid_text);
    }

    SECTION("Fail on empty string") {
        auto result = ProfessionalProfile::create("");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace only") {
        auto result = ProfessionalProfile::create("   \n\t  ");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on text shorter than minimum") {
        std::string short_text(ProfessionalProfile::kMinLength - 1, 'a');
        auto result = ProfessionalProfile::create(short_text);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidLength);
    }

    SECTION("Accept text at exactly minimum length") {
        std::string exact_min(ProfessionalProfile::kMinLength, 'b');
        auto result = ProfessionalProfile::create(exact_min);
        REQUIRE(result.has_value());
        CHECK(result->text().size() == ProfessionalProfile::kMinLength);
    }

    SECTION("Fail on text exceeding maximum") {
        std::string long_text(ProfessionalProfile::kMaxLength + 1, 'c');
        auto result = ProfessionalProfile::create(long_text);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidLength);
    }

    SECTION("Accept text at exactly maximum length") {
        std::string exact_max(ProfessionalProfile::kMaxLength, 'd');
        auto result = ProfessionalProfile::create(exact_max);
        REQUIRE(result.has_value());
        CHECK(result->text().size() == ProfessionalProfile::kMaxLength);
    }

    SECTION("Equality comparison") {
        auto a = ProfessionalProfile::create(valid_text);
        auto b = ProfessionalProfile::create(valid_text);
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(*a == *b);
    }
}
