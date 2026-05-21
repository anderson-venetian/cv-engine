#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Resultado del renderer: un documento textual listo para compilar.
// El formato (LaTeX, HTML, Markdown) lo determina el adapter del renderer.
class RenderedDocument {
public:
    enum class Format {
        Latex,
        Html,
        Markdown
    };

    [[nodiscard]] static auto create(
        Format format,
        std::string content
    ) -> common::Result<RenderedDocument>;

    [[nodiscard]] auto format()  const noexcept -> Format             { return format_; }
    [[nodiscard]] auto content() const noexcept -> const std::string& { return content_; }

    [[nodiscard]] static auto format_extension(Format f) noexcept -> std::string_view;

private:
    RenderedDocument(Format f, std::string c)
        : format_{f}, content_{std::move(c)} {}

    Format format_;
    std::string content_;
};

} // namespace cvengine::core::domain
