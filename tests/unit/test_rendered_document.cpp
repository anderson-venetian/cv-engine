#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/rendered_document.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("RenderedDocument domain entity validations", "[core][domain]") {
    using cvengine::core::domain::RenderedDocument;
    using cvengine::common::ErrorCode;
    using Format = RenderedDocument::Format;

    SECTION("Valid LaTeX document creation") {
        auto result = RenderedDocument::create(Format::Latex, "\\documentclass{article}");
        REQUIRE(result.has_value());
        CHECK(result->format() == Format::Latex);
        CHECK(result->content() == "\\documentclass{article}");
    }

    SECTION("Valid HTML document creation") {
        auto result = RenderedDocument::create(Format::Html, "<html></html>");
        REQUIRE(result.has_value());
        CHECK(result->format() == Format::Html);
    }

    SECTION("Valid Markdown document creation") {
        auto result = RenderedDocument::create(Format::Markdown, "# Title");
        REQUIRE(result.has_value());
        CHECK(result->format() == Format::Markdown);
    }

    SECTION("Fail on empty content") {
        auto result = RenderedDocument::create(Format::Latex, "");
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Format extension for LaTeX") {
        CHECK(RenderedDocument::format_extension(Format::Latex) == "tex");
    }

    SECTION("Format extension for HTML") {
        CHECK(RenderedDocument::format_extension(Format::Html) == "html");
    }

    SECTION("Format extension for Markdown") {
        CHECK(RenderedDocument::format_extension(Format::Markdown) == "md");
    }
}
