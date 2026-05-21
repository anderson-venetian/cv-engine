#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/period.hpp>
#include <cvengine/core/domain/achievement.hpp>

namespace cvengine::core::domain {

class Experience {
public:
    [[nodiscard]] static auto create(
        std::string_view position,
        std::string_view organization,
        std::string_view location,
        Period period,
        std::vector<Achievement> achievements
    ) -> common::Result<Experience>;

    [[nodiscard]] auto position()     const noexcept -> const std::string& { return position_; }
    [[nodiscard]] auto organization() const noexcept -> const std::string& { return organization_; }
    [[nodiscard]] auto location()     const noexcept -> const std::string& { return location_; }
    [[nodiscard]] auto period()       const noexcept -> const Period&      { return period_; }
    [[nodiscard]] auto achievements() const noexcept -> const std::vector<Achievement>& { return achievements_; }

    [[nodiscard]] auto operator==(const Experience&) const -> bool = default;

private:
    Experience(
        std::string position,
        std::string organization,
        std::string location,
        Period period,
        std::vector<Achievement> achievements
    )
        : position_{std::move(position)}
        , organization_{std::move(organization)}
        , location_{std::move(location)}
        , period_{std::move(period)}
        , achievements_{std::move(achievements)} {}

    std::string position_;
    std::string organization_;
    std::string location_;
    Period period_;
    std::vector<Achievement> achievements_;
};

} // namespace cvengine::core::domain
