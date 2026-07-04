#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/theme.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Theme domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Theme;
    using cvengine::common::ErrorCode;

    Theme::PropertyMap props{
        {"font_family", "Latin Modern"},
        {"font_size", "11pt"},
        {"accent_color", "#2C3E50"}
    };

    SECTION("Valid theme creation") {
        auto result = Theme::create("corporate", props);
        REQUIRE(result.has_value());
        CHECK(result->name() == "corporate");
        CHECK(result->properties().size() == 3);
    }

    SECTION("Trim whitespace from name") {
        auto result = Theme::create("  elegant  ", props);
        REQUIRE(result.has_value());
        CHECK(result->name() == "elegant");
    }

    SECTION("Fail on empty name") {
        auto result = Theme::create("", props);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on whitespace-only name") {
        auto result = Theme::create("  \t  ", props);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty properties") {
        Theme::PropertyMap empty_props;
        auto result = Theme::create("minimal", empty_props);
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Get existing property") {
        auto theme = Theme::create("test", props).value();
        CHECK(theme.get("font_size") == "11pt");
    }

    SECTION("Get missing property returns fallback") {
        auto theme = Theme::create("test", props).value();
        CHECK(theme.get("margin", "1in") == "1in");
    }

    SECTION("Get missing property returns empty by default") {
        auto theme = Theme::create("test", props).value();
        CHECK(theme.get("nonexistent").empty());
    }

    SECTION("Has returns true for existing key") {
        auto theme = Theme::create("test", props).value();
        CHECK(theme.has("accent_color"));
    }

    SECTION("Has returns false for missing key") {
        auto theme = Theme::create("test", props).value();
        CHECK_FALSE(theme.has("missing_key"));
    }
}
