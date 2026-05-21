#pragma once

#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/cv.hpp>
#include <cvengine/core/domain/theme.hpp>
#include <cvengine/core/domain/rendered_document.hpp>

namespace cvengine::core::ports {

// Port: orquesta la generación del documento intermedio (LaTeX/HTML/etc).
// Internamente usará un ITemplateEngine, pero al servicio de aplicación
// le presenta una interfaz uniforme.
class IRenderer {
public:
    IRenderer() = default;
    virtual ~IRenderer() = default;

    IRenderer(const IRenderer&) = delete;
    auto operator=(const IRenderer&) -> IRenderer& = delete;
    IRenderer(IRenderer&&) = delete;
    auto operator=(IRenderer&&) -> IRenderer& = delete;

    [[nodiscard]] virtual auto render(
        const domain::Cv& cv,
        const domain::Theme& theme
    ) const -> common::Result<domain::RenderedDocument> = 0;

    [[nodiscard]] virtual auto target_format() const noexcept
        -> domain::RenderedDocument::Format = 0;
};

} // namespace cvengine::core::ports
