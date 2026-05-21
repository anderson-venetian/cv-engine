#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Un logro es una bullet point dentro de una experiencia.
// Restricciones: no vacío, ≤ 500 caracteres (un logro más largo es un párrafo,
// señal de mal diseño del CV).
class Achievement {
public:
    static constexpr std::size_t kMaxLength = 500;

    [[nodiscard]] static auto create(std::string_view text) -> common::Result<Achievement>;

    [[nodiscard]] auto text() const noexcept -> const std::string& { return text_; }

    [[nodiscard]] auto operator==(const Achievement&) const -> bool = default;

private:
    explicit Achievement(std::string text) : text_{std::move(text)} {}
    std::string text_;
};

} // namespace cvengine::core::domain
