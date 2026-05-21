#include <print>
#include <memory>
#include <filesystem>
#include <cvengine/core/ports/i_data_repository.hpp>
#include <cvengine/core/ports/i_theme_repository.hpp>
#include <cvengine/infrastructure/persistence/json_data_repository.hpp>
#include <cvengine/infrastructure/persistence/toml_theme_repository.hpp>

namespace fs    = std::filesystem;
namespace ports = cvengine::core::ports;
namespace infra = cvengine::infrastructure::persistence;

auto main() -> int {
    std::println("cv-engine v2.0.0 — Fase 4: adapters JSON + TOML");

    // Resolución de paths: relativo al CWD donde se ejecuta el binario.
    // Convención: ejecutamos desde la raíz del proyecto.
    auto data_dir  = fs::path{"config"} / "data";
    auto theme_dir = fs::path{"config"} / "themes";

    std::println("[INFO] data_dir  = {}", data_dir.string());
    std::println("[INFO] theme_dir = {}", theme_dir.string());

    // Composition root: programamos contra interfaces, no contra implementaciones.
    std::unique_ptr<ports::IDataRepository> data_repo =
        std::make_unique<infra::JsonDataRepository>(data_dir);
    std::unique_ptr<ports::IThemeRepository> theme_repo =
        std::make_unique<infra::TomlThemeRepository>(theme_dir);

    // ----- Carga del CV -----
    auto cv_r = data_repo->load("cv_pedro");
    if (!cv_r) {
        std::println("[FAIL] DataRepository: {}", cv_r.error().message);
        return 1;
    }
    const auto& cv = *cv_r;
    std::println("[OK] CV cargado: {}", cv.contact().name());
    std::println("[OK] Email: {}", cv.contact().email());
    std::println("[OK] Profile: {} chars", cv.profile().text().size());
    std::println("[OK] Skill groups: {}", cv.skills().size());
    std::println("[OK] Experiences: {}", cv.experiences().size());
    for (const auto& exp : cv.experiences()) {
        std::println("       · {} @ {} ({} achievements)",
                     exp.position(), exp.organization(), exp.achievements().size());
    }
    std::println("[OK] Education: {} entries", cv.education().size());
    std::println("[OK] Certifications: {} entries", cv.certifications().size());

    // ----- Carga del tema -----
    auto theme_r = theme_repo->load("minimal");
    if (!theme_r) {
        std::println("[FAIL] ThemeRepository: {}", theme_r.error().message);
        return 1;
    }
    const auto& theme = *theme_r;
    std::println("[OK] Theme cargado: {}", theme.name());
    std::println("[OK] Properties: {}", theme.properties().size());
    std::println("       · page.size       = {}", theme.get("page.size"));
    std::println("       · typography.base_size = {}", theme.get("typography.base_size"));
    std::println("       · colors.accent   = {}", theme.get("colors.accent"));
    std::println("       · dividers.style  = {}", theme.get("dividers.style"));

    // ----- Test negativo: archivo inexistente -----
    auto missing = data_repo->load("no_existe");
    if (missing) {
        std::println("[FAIL] DataRepository aceptó archivo inexistente");
        return 1;
    }
    std::println("[OK] Validación de archivo inexistente: {}", missing.error().message);

    std::println("\n✓ Fase 4 completa. Adapters operativos.");
    return 0;
}
