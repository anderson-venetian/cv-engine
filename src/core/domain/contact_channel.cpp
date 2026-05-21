#include <cvengine/core/domain/contact_channel.hpp>
#include <cvengine/common/validation.hpp>

#include <algorithm>
#include <cctype>

namespace cvengine::core::domain {

namespace {

auto is_all_digits(std::string_view s) -> bool {
    return std::ranges::all_of(s, [](unsigned char c) {
        return std::isdigit(c) != 0;
    });
}

auto build_url(ContactChannel::Type type, std::string_view value) -> std::string {
    using T = ContactChannel::Type;
    switch (type) {
        case T::WhatsApp: return std::string{"https://wa.me/"} + std::string{value};
        case T::LinkedIn: return std::string{"https://linkedin.com/in/"} + std::string{value};
        case T::GitHub:   return std::string{"https://github.com/"} + std::string{value};
        case T::Twitter:  return std::string{"https://twitter.com/"} + std::string{value};
        case T::Website:
        case T::Other:    return std::string{value};
    }
    return std::string{value};
}

} // anonymous namespace

auto ContactChannel::create(
    Type type,
    std::string_view label,
    std::string_view value
) -> common::Result<ContactChannel> {
    auto label_t = common::trim(label);
    auto value_t = common::trim(value);

    if (label_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField,
            "ContactChannel: label required");
    }
    if (value_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField,
            "ContactChannel: value required");
    }

    // Validacion especifica por tipo
    if (type == Type::WhatsApp && !is_all_digits(value_t)) {
        return common::make_error(common::ErrorCode::InvalidFormat,
            "ContactChannel WhatsApp: value must be E.164 digits only (no '+', spaces)");
    }
    if ((type == Type::Website || type == Type::Other) &&
        value_t.find("://") == std::string_view::npos) {
        return common::make_error(common::ErrorCode::InvalidFormat,
            "ContactChannel Website/Other: value must be a full URL with scheme");
    }

    auto url = build_url(type, value_t);
    return ContactChannel{type, std::string{label_t}, std::string{value_t}, std::move(url)};
}

auto ContactChannel::type_from_string(std::string_view s) noexcept -> Type {
    if (s == "whatsapp") return Type::WhatsApp;
    if (s == "linkedin") return Type::LinkedIn;
    if (s == "github")   return Type::GitHub;
    if (s == "website")  return Type::Website;
    if (s == "twitter")  return Type::Twitter;
    return Type::Other;
}

auto ContactChannel::type_to_string(Type t) noexcept -> std::string_view {
    switch (t) {
        case Type::WhatsApp: return "whatsapp";
        case Type::LinkedIn: return "linkedin";
        case Type::GitHub:   return "github";
        case Type::Website:  return "website";
        case Type::Twitter:  return "twitter";
        case Type::Other:    return "other";
    }
    return "other";
}

} // namespace cvengine::core::domain
