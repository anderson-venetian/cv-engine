#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/skill_group.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("SkillGroup domain entity validations", "[core][domain]") {
    using cvengine::core::domain::SkillGroup;
    using cvengine::common::ErrorCode;

    SECTION("Valid skill group creation") {
        auto result = SkillGroup::create("Frameworks", "ITIL 4, SLA, CSAT");
        REQUIRE(result.has_value());
        CHECK(result->label() == "Frameworks");
        CHECK(result->content() == "ITIL 4, SLA, CSAT");
    }

    SECTION("Trim whitespace from all fields") {
        auto result = SkillGroup::create("  Languages  ", "  C++, Python  ");
        REQUIRE(result.has_value());
        CHECK(result->label() == "Languages");
        CHECK(result->content() == "C++, Python");
    }

    SECTION("Fail on empty label") {
        auto result = SkillGroup::create("", "Some content");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty content") {
        auto result = SkillGroup::create("Label", "");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace-only label") {
        auto result = SkillGroup::create("  \t  ", "Content");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace-only content") {
        auto result = SkillGroup::create("Label", "   \n  ");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Equality comparison") {
        auto a = SkillGroup::create("DevOps", "Docker, K8s");
        auto b = SkillGroup::create("DevOps", "Docker, K8s");
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(*a == *b);
    }
}
