#pragma once

#include <filesystem>
#include <cvengine/core/ports/i_template_engine.hpp>

namespace cvengine::infrastructure::rendering {

// Adapter de ITemplateEngine basado en inja (Jinja2-like).
// Resuelve template_name a "<base_dir>/<name>" (extensión incluida en el nombre).
// Convierte Cv + Theme a un JSON contextual antes de pasarlo a inja.
class InjaTemplateEngine final : public core::ports::ITemplateEngine {
public:
    explicit InjaTemplateEngine(std::filesystem::path templates_directory);

    [[nodiscard]] auto render(
        std::string_view template_name,
        const core::domain::Cv& cv,
        const core::domain::Theme& theme
    ) const -> common::Result<std::string> override;

private:
    std::filesystem::path templates_directory_;
};

} // namespace cvengine::infrastructure::rendering
