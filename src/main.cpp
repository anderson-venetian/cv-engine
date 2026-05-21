#include <print>
#include <memory>
#include <filesystem>
#include <cvengine/core/ports/i_data_repository.hpp>
#include <cvengine/core/ports/i_theme_repository.hpp>
#include <cvengine/core/ports/i_template_engine.hpp>
#include <cvengine/core/ports/i_renderer.hpp>
#include <cvengine/core/ports/i_compiler.hpp>
#include <cvengine/infrastructure/persistence/json_data_repository.hpp>
#include <cvengine/infrastructure/persistence/toml_theme_repository.hpp>
#include <cvengine/infrastructure/rendering/inja_template_engine.hpp>
#include <cvengine/infrastructure/rendering/latex_renderer.hpp>
#include <cvengine/infrastructure/compilation/pdflatex_compiler.hpp>

namespace fs    = std::filesystem;
namespace ports = cvengine::core::ports;
namespace dom   = cvengine::core::domain;
namespace persistence  = cvengine::infrastructure::persistence;
namespace rendering    = cvengine::infrastructure::rendering;
namespace compilation  = cvengine::infrastructure::compilation;

auto main() -> int {
    std::println("cv-engine v2.0.0 — Fase 6: flujo end-to-end (JSON+TOML → PDF)");

    // ----- Configuración de rutas -----
    auto data_dir      = fs::path{"config"} / "data";
    auto theme_dir     = fs::path{"config"} / "themes";
    auto templates_dir = fs::path{"config"} / "templates";
    auto output_dir    = fs::path{"output"};

    // ----- Composition root -----
    std::unique_ptr<ports::IDataRepository> data_repo =
        std::make_unique<persistence::JsonDataRepository>(data_dir);

    std::unique_ptr<ports::IThemeRepository> theme_repo =
        std::make_unique<persistence::TomlThemeRepository>(theme_dir);

    std::shared_ptr<ports::ITemplateEngine> template_engine =
        std::make_shared<rendering::InjaTemplateEngine>(templates_dir);

    std::unique_ptr<ports::IRenderer> renderer =
        std::make_unique<rendering::LatexRenderer>(template_engine);

    std::unique_ptr<ports::ICompiler> compiler =
        std::make_unique<compilation::PdfLatexCompiler>(
            /* executable_path     */ "",          // autodetect en PATH
            /* base_filename       */ "cv_pedro",
            /* cleanup_auxiliaries */ true,
            /* pass_count          */ 2
        );

    // ----- Fase 1: Cargar CV -----
    auto cv_r = data_repo->load("cv_pedro");
    if (!cv_r) {
        std::println("[FAIL] DataRepository: {}", cv_r.error().message);
        return 1;
    }
    std::println("[OK] CV cargado: {}", cv_r->contact().name());

    // ----- Fase 2: Cargar tema -----
    auto theme_r = theme_repo->load("minimal");
    if (!theme_r) {
        std::println("[FAIL] ThemeRepository: {}", theme_r.error().message);
        return 1;
    }
    std::println("[OK] Tema cargado: {}", theme_r->name());

    // ----- Fase 3: Renderizar LaTeX -----
    auto doc_r = renderer->render(*cv_r, *theme_r);
    if (!doc_r) {
        std::println("[FAIL] Renderer: {}", doc_r.error().message);
        return 1;
    }
    std::println("[OK] LaTeX renderizado: {} chars", doc_r->content().size());

    // ----- Fase 4: Compilar a PDF -----
    std::println("[..] Compilando con pdflatex (2 pasadas)...");
    auto pdf_r = compiler->compile(*doc_r, output_dir);
    if (!pdf_r) {
        std::println("[FAIL] Compiler: {}", pdf_r.error().message);
        return 1;
    }

    std::println("[OK] PDF generado: {}", pdf_r->string());

    // ----- Resumen -----
    std::error_code ec;
    auto size = fs::file_size(*pdf_r, ec);
    if (!ec) {
        std::println("[OK] Tamaño: {} bytes", size);
    }

    std::println("\n✓ Fase 6 completa. Flujo end-to-end operativo.");
    std::println("  Para regenerar: editar config/ → ./build/debug/bin/cv-engine.exe");
    return 0;
}
