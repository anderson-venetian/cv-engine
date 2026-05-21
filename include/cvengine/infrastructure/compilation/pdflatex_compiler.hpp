#pragma once

#include <filesystem>
#include <string>
#include <cvengine/core/ports/i_compiler.hpp>

namespace cvengine::infrastructure::compilation {

// Adapter de ICompiler que invoca pdflatex (MiKTeX o TeX Live) como subproceso.
// Estrategia:
//   1. Detecta pdflatex en PATH (sin hardcoding).
//   2. Escribe el .tex en output_directory.
//   3. Ejecuta pdflatex DOS veces (para fijar outline/referencias).
//   4. Verifica que el .pdf exista.
//   5. Limpia archivos auxiliares (.aux, .log, .out) si todo OK.
//   6. Devuelve la ruta absoluta al .pdf.
class PdfLatexCompiler final : public core::ports::ICompiler {
public:
    explicit PdfLatexCompiler(
        std::string executable_path = "",
        std::string base_filename   = "document",
        bool cleanup_auxiliaries    = true,
        int  pass_count             = 2
    );

    [[nodiscard]] auto compile(
        const core::domain::RenderedDocument& document,
        const std::filesystem::path& output_directory
    ) const -> common::Result<std::filesystem::path> override;

    [[nodiscard]] auto accepted_format() const noexcept
        -> core::domain::RenderedDocument::Format override;

private:
    std::string executable_path_;
    std::string base_filename_;
    bool        cleanup_auxiliaries_;
    int         pass_count_;
};

} // namespace cvengine::infrastructure::compilation
