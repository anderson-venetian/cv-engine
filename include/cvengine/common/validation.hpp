#pragma once

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <optional>
#include <string_view>
#include <utility>

#include <cvengine/common/error.hpp>

namespace cvengine::common {

[[nodiscard]] inline auto is_blank(std::string_view s) noexcept -> bool {
    return std::ranges::all_of(s, [](unsigned char c) {
        return std::isspace(c) != 0;
    });
}

[[nodiscard]] inline auto trim(std::string_view s) noexcept -> std::string_view {
    auto first = s.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string_view::npos) {
        return {};
    }
    auto last = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(first, last - first + 1);
}

// ---- Reglas de validación reutilizables --------------------------------
// Cada regla devuelve un Error si falla o std::nullopt si pasa, de modo que
// las factorías create() de dominio compongan validaciones sin repetir el
// patrón trim + if(empty) + make_error en cada entidad.

[[nodiscard]] inline auto require_non_empty(std::string_view value, std::string_view message)
    -> std::optional<Error> {
    if (value.empty()) {
        return Error{ErrorCode::EmptyRequiredField, message};
    }
    return std::nullopt;
}

[[nodiscard]] inline auto require_min_length(
    std::string_view value, std::size_t min_length, std::string_view message)
    -> std::optional<Error> {
    if (value.size() < min_length) {
        return Error{ErrorCode::InvalidLength, message};
    }
    return std::nullopt;
}

[[nodiscard]] inline auto require_max_length(
    std::string_view value, std::size_t max_length, std::string_view message)
    -> std::optional<Error> {
    if (value.size() > max_length) {
        return Error{ErrorCode::InvalidLength, message};
    }
    return std::nullopt;
}

// Devuelve el primer Error presente entre las reglas dadas, en orden.
// Evalúa todas las reglas (son puras y baratas) pero conserva el primero.
template <typename... Rules>
[[nodiscard]] inline auto first_error(Rules... rules) -> std::optional<Error> {
    std::optional<Error> found;
    ((found = std::move(rules)).has_value() || ...);
    return found;
}

} // namespace cvengine::common
