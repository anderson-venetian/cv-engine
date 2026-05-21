#include <cvengine/infrastructure/rendering/latex_renderer.hpp>

namespace cvengine::infrastructure::rendering {

namespace dom = core::domain;

LatexRenderer::LatexRenderer(
    std::shared_ptr<core::ports::ITemplateEngine> template_engine,
    std::string entry_template
)
    : template_engine_{std::move(template_engine)}
    , entry_template_{std::move(entry_template)} {}

auto LatexRenderer::render(
    const dom::Cv& cv,
    const dom::Theme& theme
) const -> common::Result<dom::RenderedDocument> {

    auto rendered_text = template_engine_->render(entry_template_, cv, theme);
    if (!rendered_text) {
        return std::unexpected{rendered_text.error()};
    }

    return dom::RenderedDocument::create(
        dom::RenderedDocument::Format::Latex,
        *std::move(rendered_text)
    );
}

auto LatexRenderer::target_format() const noexcept
    -> dom::RenderedDocument::Format {
    return dom::RenderedDocument::Format::Latex;
}

} // namespace cvengine::infrastructure::rendering
