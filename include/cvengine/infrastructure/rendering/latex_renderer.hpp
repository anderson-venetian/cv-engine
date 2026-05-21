#pragma once

#include <memory>
#include <string>
#include <cvengine/core/ports/i_renderer.hpp>
#include <cvengine/core/ports/i_template_engine.hpp>

namespace cvengine::infrastructure::rendering {

// Adapter de IRenderer. Delega al ITemplateEngine para resolver la plantilla
// y empaqueta el resultado en un RenderedDocument de formato Latex.
class LatexRenderer final : public core::ports::IRenderer {
public:
    explicit LatexRenderer(
        std::shared_ptr<core::ports::ITemplateEngine> template_engine,
        std::string entry_template = "base.tex.j2"
    );

    [[nodiscard]] auto render(
        const core::domain::Cv& cv,
        const core::domain::Theme& theme
    ) const -> common::Result<core::domain::RenderedDocument> override;

    [[nodiscard]] auto target_format() const noexcept
        -> core::domain::RenderedDocument::Format override;

private:
    std::shared_ptr<core::ports::ITemplateEngine> template_engine_;
    std::string entry_template_;
};

} // namespace cvengine::infrastructure::rendering
