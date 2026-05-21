#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

class Certification {
public:
    [[nodiscard]] static auto create(
        std::string_view name,
        std::string_view issuer,
        std::string_view date
    ) -> common::Result<Certification>;

    [[nodiscard]] auto name()   const noexcept -> const std::string& { return name_; }
    [[nodiscard]] auto issuer() const noexcept -> const std::string& { return issuer_; }
    [[nodiscard]] auto date()   const noexcept -> const std::string& { return date_; }

    [[nodiscard]] auto operator==(const Certification&) const -> bool = default;

private:
    Certification(std::string name, std::string issuer, std::string date)
        : name_{std::move(name)}
        , issuer_{std::move(issuer)}
        , date_{std::move(date)} {}

    std::string name_;
    std::string issuer_;
    std::string date_;
};

} // namespace cvengine::core::domain
