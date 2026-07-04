#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/education.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Education domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Education;
    using cvengine::common::ErrorCode;

    SECTION("Valid education creation") {
        auto result = Education::create(
            "Computacion e Informatica",
            "ISTP Argentina",
            "Lima, Peru",
            "2024"
        );
        REQUIRE(result.has_value());
        CHECK(result->degree() == "Computacion e Informatica");
        CHECK(result->institution() == "ISTP Argentina");
        CHECK(result->location() == "Lima, Peru");
        CHECK(result->completion_date() == "2024");
    }

    SECTION("Trim whitespace from all fields") {
        auto result = Education::create(
            "  CS  ",
            "  MIT  ",
            "  Boston  ",
            "  2023  "
        );
        REQUIRE(result.has_value());
        CHECK(result->degree() == "CS");
        CHECK(result->institution() == "MIT");
        CHECK(result->location() == "Boston");
        CHECK(result->completion_date() == "2023");
    }

    SECTION("Allow empty location") {
        auto result = Education::create("Degree", "Institution", "", "2024");
        REQUIRE(result.has_value());
        CHECK(result->location().empty());
    }

    SECTION("Fail on empty degree") {
        auto result = Education::create("", "Institution", "City", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty institution") {
        auto result = Education::create("Degree", "", "City", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty completion date") {
        auto result = Education::create("Degree", "Institution", "City", "");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace-only degree") {
        auto result = Education::create("  \t  ", "Institution", "City", "2024");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Equality comparison") {
        auto a = Education::create("CS", "MIT", "Boston", "2023");
        auto b = Education::create("CS", "MIT", "Boston", "2023");
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(*a == *b);
    }
}
