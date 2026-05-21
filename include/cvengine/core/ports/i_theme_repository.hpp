#pragma once

#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/theme.hpp>

namespace cvengine::core::ports {

// Port: carga un tema desde una fuente persistente.
class IThemeRepository {
public:
    IThemeRepository() = default;
    virtual ~IThemeRepository() = default;

    IThemeRepository(const IThemeRepository&) = delete;
    auto operator=(const IThemeRepository&) -> IThemeRepository& = delete;
    IThemeRepository(IThemeRepository&&) = delete;
    auto operator=(IThemeRepository&&) -> IThemeRepository& = delete;

    [[nodiscard]] virtual auto load(std::string_view name) const
        -> common::Result<domain::Theme> = 0;
};

} // namespace cvengine::core::ports
