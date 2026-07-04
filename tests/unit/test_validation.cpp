#include <catch2/catch_test_macros.hpp>
#include <cvengine/common/validation.hpp>

TEST_CASE("Validation utility: is_blank", "[common]") {
    using cvengine::common::is_blank;

    SECTION("Empty string is blank") {
        CHECK(is_blank(""));
    }

    SECTION("Whitespace-only is blank") {
        CHECK(is_blank("   "));
        CHECK(is_blank("\t\n\r "));
    }

    SECTION("Non-blank strings") {
        CHECK_FALSE(is_blank("hello"));
        CHECK_FALSE(is_blank(" a "));
    }
}

TEST_CASE("Validation utility: trim", "[common]") {
    using cvengine::common::trim;

    SECTION("No trimming needed") {
        CHECK(trim("hello") == "hello");
    }

    SECTION("Trim leading whitespace") {
        CHECK(trim("  hello") == "hello");
    }

    SECTION("Trim trailing whitespace") {
        CHECK(trim("hello  ") == "hello");
    }

    SECTION("Trim both sides") {
        CHECK(trim("  hello  ") == "hello");
    }

    SECTION("Trim tabs and newlines") {
        CHECK(trim("\t\nhello\r\n") == "hello");
    }

    SECTION("Empty string returns empty") {
        CHECK(trim("").empty());
    }

    SECTION("Whitespace-only returns empty") {
        CHECK(trim("   \t\n  ").empty());
    }

    SECTION("Preserve inner whitespace") {
        CHECK(trim("  hello world  ") == "hello world");
    }
}
