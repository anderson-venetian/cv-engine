#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/achievement.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Achievement domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Achievement;
    using cvengine::common::ErrorCode;

    SECTION("Valid achievement creation") {
        auto result = Achievement::create("Implemented SLA monitoring system");
        REQUIRE(result.has_value());
        CHECK(result->text() == "Implemented SLA monitoring system");
    }

    SECTION("Trim whitespace") {
        auto result = Achievement::create("  Reduced ticket backlog by 40%  ");
        REQUIRE(result.has_value());
        CHECK(result->text() == "Reduced ticket backlog by 40%");
    }

    SECTION("Fail on empty string") {
        auto result = Achievement::create("");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace only") {
        auto result = Achievement::create("   \t\n  ");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on text exceeding max length") {
        std::string long_text(Achievement::kMaxLength + 1, 'x');
        auto result = Achievement::create(long_text);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidLength);
    }

    SECTION("Accept text at exactly max length") {
        std::string exact_text(Achievement::kMaxLength, 'a');
        auto result = Achievement::create(exact_text);
        REQUIRE(result.has_value());
        CHECK(result->text().size() == Achievement::kMaxLength);
    }

    SECTION("Equality comparison") {
        auto a = Achievement::create("Same text");
        auto b = Achievement::create("Same text");
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(*a == *b);
    }
}
