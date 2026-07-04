#include <catch2/catch_test_macros.hpp>
#include <cvengine/infrastructure/rendering/latex_escape.hpp>

TEST_CASE("LaTeX escape special characters", "[infrastructure][rendering]") {
    using cvengine::infrastructure::rendering::escape_latex;

    SECTION("Plain text unchanged") {
        CHECK(escape_latex("Hello World") == "Hello World");
    }

    SECTION("Empty string") {
        CHECK(escape_latex("").empty());
    }

    SECTION("Escape ampersand") {
        CHECK(escape_latex("A & B") == "A \\& B");
    }

    SECTION("Escape percent") {
        CHECK(escape_latex("100%") == "100\\%");
    }

    SECTION("Escape dollar") {
        CHECK(escape_latex("$100") == "\\$100");
    }

    SECTION("Escape hash") {
        CHECK(escape_latex("item #1") == "item \\#1");
    }

    SECTION("Escape underscore") {
        CHECK(escape_latex("file_name") == "file\\_name");
    }

    SECTION("Escape braces") {
        CHECK(escape_latex("{text}") == "\\{text\\}");
    }

    SECTION("Escape tilde") {
        CHECK(escape_latex("~user") == "\\textasciitilde{}user");
    }

    SECTION("Escape caret") {
        CHECK(escape_latex("x^2") == "x\\textasciicircum{}2");
    }

    SECTION("Escape backslash") {
        CHECK(escape_latex("path\\to") == "path\\textbackslash{}to");
    }

    SECTION("Multiple special characters in one string") {
        CHECK(escape_latex("50% & $10") == "50\\% \\& \\$10");
    }

    SECTION("All special characters combined") {
        auto result = escape_latex("\\&%$#_{}~^");
        CHECK(result == "\\textbackslash{}\\&\\%\\$\\#\\_\\{\\}\\textasciitilde{}\\textasciicircum{}");
    }
}
