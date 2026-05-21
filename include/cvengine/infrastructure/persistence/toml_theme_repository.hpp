#pragma once

#include <filesystem>
#include <cvengine/core/ports/i_theme_repository.hpp>

namespace cvengine::infrastructure::persistence {

// Adapter de IThemeRepository que lee temas desde archivos TOML.
// El name "corporate" se resuelve a "<base_dir>/corporate.toml".
// El TOML jerárquico se aplana a claves dotted: [colors] primary = "#x" → "colors.primary".
class TomlThemeRepository final : public core::ports::IThemeRepository {
public:
    explicit TomlThemeRepository(std::filesystem::path base_directory);

    [[nodiscard]] auto load(std::string_view name) const
        -> common::Result<core::domain::Theme> override;

private:
    std::filesystem::path base_directory_;
};

} // namespace cvengine::infrastructure::persistence
