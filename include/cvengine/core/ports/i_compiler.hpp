#pragma once

#include <filesystem>
#include <future>
#include <cvengine/common/result.hpp>
#include <cvengine/core/domain/rendered_document.hpp>

namespace cvengine::core::ports {

// Port: compila un documento intermedio a su formato final (típicamente PDF).
// Implementaciones: pdflatex, weasyprint, pandoc, etc.
class ICompiler {
public:
    ICompiler() = default;
    virtual ~ICompiler() = default;

    ICompiler(const ICompiler&) = delete;
    auto operator=(const ICompiler&) -> ICompiler& = delete;
    ICompiler(ICompiler&&) = delete;
    auto operator=(ICompiler&&) -> ICompiler& = delete;

    // Toma el documento intermedio y un directorio de trabajo.
    // Devuelve la ruta absoluta al PDF generado.
    [[nodiscard]] virtual auto compile(
        const domain::RenderedDocument& document,
        const std::filesystem::path& output_directory
    ) const -> common::Result<std::filesystem::path> = 0;

    // Ejecuta la compilación de forma asíncrona no bloqueante.
    [[nodiscard]] virtual auto compile_async(
        domain::RenderedDocument document,
        std::filesystem::path output_directory
    ) const -> std::future<common::Result<std::filesystem::path>> {
        return std::async(std::launch::async, [this, doc = std::move(document), dir = std::move(output_directory)]() {
            return this->compile(doc, dir);
        });
    }

    // Indica qué formato de entrada acepta este compilador.
    [[nodiscard]] virtual auto accepted_format() const noexcept
        -> domain::RenderedDocument::Format = 0;
};

} // namespace cvengine::core::ports
