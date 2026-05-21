#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

class Contact {
public:
    [[nodiscard]] static auto create(
        std::string_view name,
        std::string_view location,
        std::string_view phone,
        std::string_view email
    ) -> common::Result<Contact>;

    [[nodiscard]] auto name()     const noexcept -> const std::string& { return name_; }
    [[nodiscard]] auto location() const noexcept -> const std::string& { return location_; }
    [[nodiscard]] auto phone()    const noexcept -> const std::string& { return phone_; }
    [[nodiscard]] auto email()    const noexcept -> const std::string& { return email_; }

    [[nodiscard]] auto operator==(const Contact&) const -> bool = default;

private:
    Contact(std::string name, std::string location, std::string phone, std::string email)
        : name_{std::move(name)}
        , location_{std::move(location)}
        , phone_{std::move(phone)}
        , email_{std::move(email)} {}

    std::string name_;
    std::string location_;
    std::string phone_;
    std::string email_;
};

} // namespace cvengine::core::domain
