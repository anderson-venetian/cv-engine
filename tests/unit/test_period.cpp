#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/period.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Period domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Period;
    using cvengine::common::ErrorCode;

    SECTION("Valid period creation") {
        auto result = Period::create("Mar 2026 — Abr 2026");
        REQUIRE(result.has_value());
        CHECK(result->value() == "Mar 2026 — Abr 2026");
    }

    SECTION("Trim whitespace") {
        auto result = Period::create("  Ene 2025  ");
        REQUIRE(result.has_value());
        CHECK(result->value() == "Ene 2025");
    }

    SECTION("Fail on empty string") {
        auto result = Period::create("");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace only") {
        auto result = Period::create("   \n\t ");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on too long period text") {
        std::string long_text(101, 'a');
        auto result = Period::create(long_text);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidLength);
    }
}
