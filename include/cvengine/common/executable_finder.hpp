#pragma once

#include <filesystem>
#include <optional>
#include <string_view>

namespace cvengine::common {

// Busca un ejecutable por nombre. Estrategia:
//  1. Si name es ruta absoluta → verificar existencia.
//  2. Buscar en cada entrada de la variable PATH del entorno.
//  3. En Windows: probar también extensión .exe.
// Devuelve la ruta absoluta si lo encuentra, vacío en caso contrario.
[[nodiscard]] auto find_executable(std::string_view name)
    -> std::optional<std::filesystem::path>;

} // namespace cvengine::common
