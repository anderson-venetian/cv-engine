#include <memory>
#include <filesystem>

#include <cvengine/application/use_cases/generate_cv_use_case.hpp>
#include <cvengine/presentation/cli/cli_application.hpp>
#include <cvengine/infrastructure/persistence/json_data_repository.hpp>
#include <cvengine/infrastructure/persistence/toml_theme_repository.hpp>
#include <cvengine/infrastructure/rendering/inja_template_engine.hpp>
#include <cvengine/infrastructure/rendering/latex_renderer.hpp>
#include <cvengine/infrastructure/compilation/pdflatex_compiler.hpp>

// Composition root: unico lugar donde se cablea el grafo de dependencias.
// Toda la logica de negocio vive en application; toda la I/O en infrastructure.
auto main(int argc, char** argv) -> int {
    namespace fs    = std::filesystem;
    namespace infra = cvengine::infrastructure;
    namespace app   = cvengine::application;
    namespace pres  = cvengine::presentation;

    auto data_dir      = fs::path{"config"} / "data";
    auto theme_dir     = fs::path{"config"} / "themes";
    auto templates_dir = fs::path{"config"} / "templates";

    auto data_repo = std::make_shared<infra::persistence::JsonDataRepository>(data_dir);
    auto theme_repo = std::make_shared<infra::persistence::TomlThemeRepository>(theme_dir);
    auto template_engine = std::make_shared<infra::rendering::InjaTemplateEngine>(templates_dir);
    auto renderer = std::make_shared<infra::rendering::LatexRenderer>(template_engine);

    // El nombre del archivo PDF se deriva del data_identifier (ej: "cv_pedro" -> "cv_pedro.pdf").
    // Lo cierto: por ahora hardcodeamos "cv_pedro"; en una iteracion futura
    // el compiler aceptara el nombre desde el request.
    auto compiler = std::make_shared<infra::compilation::PdfLatexCompiler>(
        "",          // pdflatex autodetect
        "cv_pedro",  // base filename - TODO: derivarlo del request
        true,        // cleanup
        2            // passes
    );

    auto use_case = std::make_shared<app::use_cases::GenerateCvUseCase>(
        data_repo, theme_repo, renderer, compiler
    );

    pres::cli::CliApplication cli{use_case};
    return cli.run(argc, argv);
}
