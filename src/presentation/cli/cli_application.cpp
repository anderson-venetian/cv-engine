#include <cvengine/presentation/cli/cli_application.hpp>

#include <print>
#include <string>
#include <filesystem>
#include <CLI/CLI.hpp>
#include <cvengine/common/error.hpp>

namespace cvengine::presentation::cli {

namespace fs = std::filesystem;
namespace uc = application::use_cases;

namespace {

auto map_error_to_exit_code(common::ErrorCode code) -> int {
    using common::ErrorCode;
    switch (code) {
        case ErrorCode::CompilationError:
            return ExitCode::CompilationError;
        case ErrorCode::IoError:
        case ErrorCode::FileNotFound:
            return ExitCode::IoError;
        case ErrorCode::EmptyRequiredField:
        case ErrorCode::InvalidFormat:
        case ErrorCode::InvalidLength:
        case ErrorCode::InvalidDateRange:
        case ErrorCode::ParseError:
            return ExitCode::ConfigurationError;
        case ErrorCode::RenderError:
            return ExitCode::ConfigurationError;
        case ErrorCode::DuplicateEntry:
            return ExitCode::ConfigurationError;
        case ErrorCode::Unknown:
        default:
            return ExitCode::UnknownError;
    }
}

} // anonymous namespace

CliApplication::CliApplication(
    std::shared_ptr<uc::GenerateCvUseCase> use_case
)
    : use_case_{std::move(use_case)} {}

auto CliApplication::run(int argc, char** argv) const -> int {

    CLI::App app{"cv-engine: generador modular de CV en LaTeX (v2.0.0)"};
    app.set_version_flag("--version", "cv-engine 2.0.0");

    std::string data_id     = "cv_pedro";
    std::string theme_name  = "minimal";
    std::string output_path = "output";
    bool        verbose     = false;

    app.add_option("-d,--data", data_id,
        "Identificador del CV (nombre del archivo JSON sin extension)")
        ->default_val("cv_pedro")
        ->capture_default_str();

    app.add_option("-t,--theme", theme_name,
        "Nombre del tema (archivo TOML sin extension)")
        ->default_val("minimal")
        ->capture_default_str();

    app.add_option("-o,--output", output_path,
        "Directorio de salida para el PDF")
        ->default_val("output")
        ->capture_default_str();

    app.add_flag("-v,--verbose", verbose,
        "Muestra detalles del proceso de generacion");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    if (verbose) {
        std::println("cv-engine v2.0.0");
        std::println("  data    : {}", data_id);
        std::println("  theme   : {}", theme_name);
        std::println("  output  : {}", output_path);
        std::println("");
    }

    uc::GenerateCvRequest request{
        .data_identifier = data_id,
        .theme_name      = theme_name,
        .output_dir      = fs::path{output_path}
    };

    auto result = use_case_->execute(request);
    if (!result) {
        std::println(stderr, "[ERROR] {}", result.error().message);
        return map_error_to_exit_code(result.error().code);
    }

    if (verbose) {
        std::println("[OK] PDF generado: {}", result->pdf_path.string());
        std::println("[OK] Tamano: {} bytes", result->pdf_bytes);
    } else {
        std::println("{}", result->pdf_path.string());
    }

    return ExitCode::Success;
}

} // namespace cvengine::presentation::cli
