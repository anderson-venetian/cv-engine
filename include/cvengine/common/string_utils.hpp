#pragma once

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace cvengine::common {

// Divide `text` en tokens usando `delimiter`. Por defecto descarta los
// tokens vacíos (p. ej. separadores consecutivos o al inicio/final).
[[nodiscard]] inline auto split(
    std::string_view text, char delimiter, bool skip_empty = true)
    -> std::vector<std::string> {
    std::vector<std::string> parts;
    std::stringstream stream{std::string{text}};
    std::string item;
    while (std::getline(stream, item, delimiter)) {
        if (skip_empty && item.empty()) {
            continue;
        }
        parts.push_back(std::move(item));
    }
    return parts;
}

// Concatena todos los argumentos en una sola cadena vía operator<<.
// Reemplaza el patrón repetido de std::ostringstream + << + .str()
// usado para construir mensajes de error dinámicos.
template <typename... Parts>
[[nodiscard]] auto concat(const Parts&... parts) -> std::string {
    std::ostringstream stream;
    (stream << ... << parts);
    return stream.str();
}

} // namespace cvengine::common
