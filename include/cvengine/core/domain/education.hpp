#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

class Education {
public:
    [[nodiscard]] static auto create(
        std::string_view degree,
        std::string_view institution,
        std::string_view location,
        std::string_view completion_date
    ) -> common::Result<Education>;

    [[nodiscard]] auto degree()          const noexcept -> const std::string& { return degree_; }
    [[nodiscard]] auto institution()     const noexcept -> const std::string& { return institution_; }
    [[nodiscard]] auto location()        const noexcept -> const std::string& { return location_; }
    [[nodiscard]] auto completion_date() const noexcept -> const std::string& { return completion_date_; }

    [[nodiscard]] auto operator==(const Education&) const -> bool = default;

private:
    Education(std::string degree, std::string institution, std::string location, std::string completion_date)
        : degree_{std::move(degree)}
        , institution_{std::move(institution)}
        , location_{std::move(location)}
        , completion_date_{std::move(completion_date)} {}

    std::string degree_;
    std::string institution_;
    std::string location_;
    std::string completion_date_;
};

} // namespace cvengine::core::domain
