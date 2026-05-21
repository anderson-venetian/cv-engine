#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Representa un canal de contacto adicional al telefono/email basicos.
// Disenado para extension: cada nuevo tipo de canal agrega un valor
// al enum y registra su prefijo de URL en build_url().
class ContactChannel {
public:
    enum class Type {
        WhatsApp,    // value = numero E.164 sin '+', url = https://wa.me/<value>
        LinkedIn,    // value = handle, url = https://linkedin.com/in/<value>
        GitHub,      // value = handle, url = https://github.com/<value>
        Website,     // value = URL completa con https://
        Twitter,     // value = handle, url = https://twitter.com/<value>
        Other        // value = URL completa con https://
    };

    [[nodiscard]] static auto create(
        Type type,
        std::string_view label,
        std::string_view value
    ) -> common::Result<ContactChannel>;

    [[nodiscard]] static auto type_from_string(std::string_view s) noexcept -> Type;
    [[nodiscard]] static auto type_to_string(Type t) noexcept -> std::string_view;

    [[nodiscard]] auto type()  const noexcept -> Type               { return type_; }
    [[nodiscard]] auto label() const noexcept -> const std::string& { return label_; }
    [[nodiscard]] auto value() const noexcept -> const std::string& { return value_; }
    [[nodiscard]] auto url()   const noexcept -> const std::string& { return url_; }

    [[nodiscard]] auto operator==(const ContactChannel&) const -> bool = default;

private:
    ContactChannel(Type type, std::string label, std::string value, std::string url)
        : type_{type}
        , label_{std::move(label)}
        , value_{std::move(value)}
        , url_{std::move(url)} {}

    Type type_;
    std::string label_;
    std::string value_;
    std::string url_;
};

} // namespace cvengine::core::domain
