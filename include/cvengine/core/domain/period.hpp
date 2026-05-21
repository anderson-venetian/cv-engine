#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Representa un rango temporal como string libre (ej: "Mar 2026 — Abr 2026").
// Razón: los formatos de fecha en CVs varían enormemente por idioma y estilo.
// Forzar std::chrono perdería expresividad ("Actualidad", "Verano 2025").
// Validamos solo: no vacío, longitud razonable.
class Period {
public:
    [[nodiscard]] static auto create(std::string_view text) -> common::Result<Period>;

    [[nodiscard]] auto value() const noexcept -> const std::string& { return text_; }

    [[nodiscard]] auto operator==(const Period&) const -> bool = default;

private:
    explicit Period(std::string text) : text_{std::move(text)} {}
    std::string text_;
};

} // namespace cvengine::core::domain
