#include <print>
#include <memory>
#include <filesystem>
#include <fstream>
#include <cvengine/core/ports/i_data_repository.hpp>
#include <cvengine/core/ports/i_theme_repository.hpp>
#include <cvengine/core/ports/i_template_engine.hpp>
#include <cvengine/core/ports/i_renderer.hpp>
#include <cvengine/infrastructure/persistence/json_data_repository.hpp>
#include <cvengine/infrastructure/persistence/toml_theme_repository.hpp>
#include <cvengine/infrastructure/rendering/inja_template_engine.hpp>
#include <cvengine/infrastructure/rendering/latex_renderer.hpp>

namespace fs    = std::filesystem;
namespace ports = cvengine::core::ports;
namespace dom   = cvengine::core::domain;
namespace persistence = cvengine::infrastructure::persistence;
namespace rendering   = cvengine::infrastructure::rendering;

auto main() -> int {
    std::println("cv-engine v2.0.0 — Fase 5: templating + renderer LaTeX");

    auto data_dir      = fs::path{"config"} / "data";
    auto theme_dir     = fs::path{"config"} / "themes";
    auto templates_dir = fs::path{"config"} / "templates";
    auto output_dir    = fs::path{"output"};

    fs::create_directories(output_dir);

    // ----- Composition root -----
    std::unique_ptr<ports::IDataRepository> data_repo =
        std::make_unique<persistence::JsonDataRepository>(data_dir);

    std::unique_ptr<ports::IThemeRepository> theme_repo =
        std::make_unique<persistence::TomlThemeRepository>(theme_dir);

    std::shared_ptr<ports::ITemplateEngine> template_engine =
        std::make_shared<rendering::InjaTemplateEngine>(templates_dir);

    std::unique_ptr<ports::IRenderer> renderer =
        std::make_unique<rendering::LatexRenderer>(template_engine);

    // ----- Carga -----
    auto cv_r = data_repo->load("cv_pedro");
    if (!cv_r) {
        std::println("[FAIL] DataRepository: {}", cv_r.error().message);
        return 1;
    }
    std::println("[OK] CV cargado: {}", cv_r->contact().name());

    auto theme_r = theme_repo->load("minimal");
    if (!theme_r) {
        std::println("[FAIL] ThemeRepository: {}", theme_r.error().message);
        return 1;
    }
    std::println("[OK] Theme cargado: {}", theme_r->name());

    // ----- Render -----
    auto doc_r = renderer->render(*cv_r, *theme_r);
    if (!doc_r) {
        std::println("[FAIL] Renderer: {}", doc_r.error().message);
        return 1;
    }
    const auto& doc = *doc_r;
    std::println("[OK] LaTeX renderizado: {} chars", doc.content().size());
    std::println("[OK] Formato: {}",
                 dom::RenderedDocument::format_extension(doc.format()));

    // ----- Guardar a archivo -----
    auto output_path = output_dir / "cv_pedro.tex";
    std::ofstream out{output_path};
    if (!out) {
        std::println("[FAIL] No se pudo abrir {}", output_path.string());
        return 1;
    }
    out << doc.content();
    out.close();
    std::println("[OK] Archivo escrito: {}", output_path.string());

    // ----- Vista previa de las primeras líneas -----
    std::println("\n--- Preview (primeras 8 líneas del .tex) ---");
    std::ifstream preview{output_path};
    std::string line;
    int line_count = 0;
    while (std::getline(preview, line) && line_count < 8) {
        std::println("{:>3} | {}", ++line_count, line);
    }
    std::println("--- (continúa) ---");

    std::println("\n✓ Fase 5 completa. LaTeX listo para compilar a PDF en Fase 6.");
    return 0;
}
