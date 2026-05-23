#pragma once

#include <memory>
#include <string>
#include <filesystem>
#include <future>
#include <cvengine/common/result.hpp>
#include <cvengine/core/ports/i_data_repository.hpp>
#include <cvengine/core/ports/i_theme_repository.hpp>
#include <cvengine/core/ports/i_renderer.hpp>
#include <cvengine/core/ports/i_compiler.hpp>

namespace cvengine::application::use_cases {

// Parámetros de entrada del caso de uso.
struct GenerateCvRequest {
    std::string data_identifier;      // p.ej. "cv_pedro"
    std::string theme_name;            // p.ej. "minimal"
    std::filesystem::path output_dir;  // donde guardar el PDF
};

// Resultado: ruta absoluta al PDF generado.
struct GenerateCvResponse {
    std::filesystem::path pdf_path;
    std::size_t pdf_bytes;
};

// Caso de uso: orquesta repositorios, renderer y compiler.
// No conoce JSON, TOML, LaTeX ni pdflatex: solo interfaces.
class GenerateCvUseCase {
public:
    GenerateCvUseCase(
        std::shared_ptr<core::ports::IDataRepository>  data_repo,
        std::shared_ptr<core::ports::IThemeRepository> theme_repo,
        std::shared_ptr<core::ports::IRenderer>        renderer,
        std::shared_ptr<core::ports::ICompiler>        compiler
    );

    [[nodiscard]] auto execute(const GenerateCvRequest& request) const
        -> common::Result<GenerateCvResponse>;

    // Variante asíncrona del caso de uso
    [[nodiscard]] auto execute_async(const GenerateCvRequest& request) const
        -> std::future<common::Result<GenerateCvResponse>>;

private:
    std::shared_ptr<core::ports::IDataRepository>  data_repo_;
    std::shared_ptr<core::ports::IThemeRepository> theme_repo_;
    std::shared_ptr<core::ports::IRenderer>        renderer_;
    std::shared_ptr<core::ports::ICompiler>        compiler_;
};

} // namespace cvengine::application::use_cases
