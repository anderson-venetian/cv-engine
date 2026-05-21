#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/cv.hpp>
#include <cvengine/core/domain/theme.hpp>

namespace cvengine::core::ports {

// Port: motor de plantillas. Toma una plantilla y datos, devuelve texto.
// El formato del template es opaco al dominio (puede ser Jinja2, Mustache, etc).
class ITemplateEngine {
public:
    ITemplateEngine() = default;
    virtual ~ITemplateEngine() = default;

    ITemplateEngine(const ITemplateEngine&) = delete;
    auto operator=(const ITemplateEngine&) -> ITemplateEngine& = delete;
    ITemplateEngine(ITemplateEngine&&) = delete;
    auto operator=(ITemplateEngine&&) -> ITemplateEngine& = delete;

    // Renderiza una plantilla (identificada por nombre lógico) con CV + Theme.
    // El adapter resuelve "base" -> "config/templates/base.tex.j2" o equivalente.
    [[nodiscard]] virtual auto render(
        std::string_view template_name,
        const domain::Cv& cv,
        const domain::Theme& theme
    ) const -> common::Result<std::string> = 0;
};

} // namespace cvengine::core::ports
