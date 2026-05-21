#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Theme: configuración estética de un CV.
// Modelado como bag of properties (key -> value) para máxima flexibilidad:
// el dominio no decide qué claves existen — eso lo determina la plantilla.
// El theme repository validará contra el schema.toml en su capa.
class Theme {
public:
    using PropertyMap = std::unordered_map<std::string, std::string>;

    [[nodiscard]] static auto create(
        std::string_view name,
        PropertyMap properties
    ) -> common::Result<Theme>;

    [[nodiscard]] auto name()       const noexcept -> const std::string&  { return name_; }
    [[nodiscard]] auto properties() const noexcept -> const PropertyMap&  { return properties_; }

    // Acceso conveniente con default.
    [[nodiscard]] auto get(std::string_view key, std::string_view fallback = "") const -> std::string;
    [[nodiscard]] auto has(std::string_view key) const noexcept -> bool;

private:
    Theme(std::string name, PropertyMap properties)
        : name_{std::move(name)}, properties_{std::move(properties)} {}

    std::string name_;
    PropertyMap properties_;
};

} // namespace cvengine::core::domain
