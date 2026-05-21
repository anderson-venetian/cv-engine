#pragma once

#include <filesystem>
#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/cv.hpp>

namespace cvengine::core::ports {

// Port: carga un CV desde una fuente persistente.
// Implementaciones posibles: JSON, YAML, base de datos, memoria.
class IDataRepository {
public:
    IDataRepository() = default;
    virtual ~IDataRepository() = default;

    IDataRepository(const IDataRepository&) = delete;
    auto operator=(const IDataRepository&) -> IDataRepository& = delete;
    IDataRepository(IDataRepository&&) = delete;
    auto operator=(IDataRepository&&) -> IDataRepository& = delete;

    // Carga un CV identificado por nombre lógico (ej: "cv_pedro").
    // El adapter resuelve el nombre a una ruta/conexión concreta.
    [[nodiscard]] virtual auto load(std::string_view identifier) const
        -> common::Result<domain::Cv> = 0;
};

} // namespace cvengine::core::ports
