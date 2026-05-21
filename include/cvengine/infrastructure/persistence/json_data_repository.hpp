#pragma once

#include <filesystem>
#include <cvengine/core/ports/i_data_repository.hpp>

namespace cvengine::infrastructure::persistence {

// Adapter de IDataRepository que lee CVs desde archivos JSON.
// El identifier "cv_pedro" se resuelve a "<base_dir>/cv_pedro.json".
class JsonDataRepository final : public core::ports::IDataRepository {
public:
    explicit JsonDataRepository(std::filesystem::path base_directory);

    [[nodiscard]] auto load(std::string_view identifier) const
        -> common::Result<core::domain::Cv> override;

private:
    std::filesystem::path base_directory_;
};

} // namespace cvengine::infrastructure::persistence
