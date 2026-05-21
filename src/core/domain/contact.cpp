#include <cvengine/core/domain/contact.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Contact::create(
    std::string_view name,
    std::string_view location,
    std::string_view phone,
    std::string_view email,
    std::vector<ContactChannel> extra_channels
) -> common::Result<Contact> {
    auto name_t     = common::trim(name);
    auto location_t = common::trim(location);
    auto phone_t    = common::trim(phone);
    auto email_t    = common::trim(email);

    if (name_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField,
            "Contact: name is required");
    }
    if (location_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField,
            "Contact: location is required");
    }
    if (email_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField,
            "Contact: email is required");
    }
    if (email_t.find('@') == std::string_view::npos) {
        return common::make_error(common::ErrorCode::InvalidFormat,
            "Contact: email must contain '@'");
    }

    return Contact{
        std::string{name_t},
        std::string{location_t},
        std::string{phone_t},
        std::string{email_t},
        std::move(extra_channels)
    };
}

} // namespace cvengine::core::domain
