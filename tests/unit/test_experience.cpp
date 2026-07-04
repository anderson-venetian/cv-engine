#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/experience.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Experience domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Experience;
    using cvengine::core::domain::Period;
    using cvengine::core::domain::Achievement;
    using cvengine::common::ErrorCode;

    auto period = Period::create("2020 - 2023").value();
    auto achievement = Achievement::create("Managed 500+ tickets monthly").value();
    std::vector<Achievement> achievements{achievement};

    SECTION("Valid experience creation") {
        auto result = Experience::create(
            "Service Desk Analyst",
            "Tech Corp",
            "Lima, Peru",
            period,
            achievements
        );
        REQUIRE(result.has_value());
        CHECK(result->position() == "Service Desk Analyst");
        CHECK(result->organization() == "Tech Corp");
        CHECK(result->location() == "Lima, Peru");
        CHECK(result->period() == period);
        CHECK(result->achievements().size() == 1);
    }

    SECTION("Trim whitespace from fields") {
        auto result = Experience::create(
            "  Dev  ",
            "  Company  ",
            "  City  ",
            period,
            achievements
        );
        REQUIRE(result.has_value());
        CHECK(result->position() == "Dev");
        CHECK(result->organization() == "Company");
        CHECK(result->location() == "City");
    }

    SECTION("Fail on empty position") {
        auto result = Experience::create("", "Org", "City", period, achievements);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty organization") {
        auto result = Experience::create("Pos", "", "City", period, achievements);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty location") {
        auto result = Experience::create("Pos", "Org", "", period, achievements);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty achievements") {
        std::vector<Achievement> empty_achievements;
        auto result = Experience::create("Pos", "Org", "City", period, empty_achievements);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Multiple achievements") {
        auto ach2 = Achievement::create("Reduced MTTR by 30%").value();
        std::vector<Achievement> multi{achievement, ach2};
        auto result = Experience::create("Analyst", "Corp", "Lima", period, multi);
        REQUIRE(result.has_value());
        CHECK(result->achievements().size() == 2);
    }

    SECTION("Fail on whitespace-only position") {
        auto result = Experience::create("  \t  ", "Org", "City", period, achievements);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }
}
