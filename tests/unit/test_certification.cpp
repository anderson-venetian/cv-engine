#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/certification.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Certification domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Certification;
    using cvengine::common::ErrorCode;

    SECTION("Valid certification creation") {
        auto result = Certification::create("ITIL 4 Foundation", "PeopleCert", "2024");
        REQUIRE(result.has_value());
        CHECK(result->name() == "ITIL 4 Foundation");
        CHECK(result->issuer() == "PeopleCert");
        CHECK(result->date() == "2024");
    }

    SECTION("Trim whitespace from all fields") {
        auto result = Certification::create("  AWS SAA  ", "  Amazon  ", "  2023  ");
        REQUIRE(result.has_value());
        CHECK(result->name() == "AWS SAA");
        CHECK(result->issuer() == "Amazon");
        CHECK(result->date() == "2023");
    }

    SECTION("Fail on empty name") {
        auto result = Certification::create("", "Issuer", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty issuer") {
        auto result = Certification::create("Cert", "", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty date") {
        auto result = Certification::create("Cert", "Issuer", "");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace-only name") {
        auto result = Certification::create("   ", "Issuer", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Equality comparison") {
        auto a = Certification::create("ITIL 4", "PeopleCert", "2024");
        auto b = Certification::create("ITIL 4", "PeopleCert", "2024");
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(*a == *b);
    }
}
