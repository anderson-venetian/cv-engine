#pragma once

#include <memory>
#include <cvengine/application/use_cases/generate_cv_use_case.hpp>

namespace cvengine::presentation::cli {

// Exit codes públicos. Usables por scripts.
struct ExitCode {
    static constexpr int Success            = 0;
    static constexpr int ConfigurationError = 1;  // argumentos invalidos, archivo no encontrado
    static constexpr int CompilationError   = 2;  // pdflatex fallo
    static constexpr int IoError            = 3;  // problemas de disco
    static constexpr int UnknownError       = 4;
};

class CliApplication {
public:
    explicit CliApplication(
        std::shared_ptr<application::use_cases::GenerateCvUseCase> use_case
    );

    // Parsea argumentos, ejecuta el caso de uso, retorna exit code.
    [[nodiscard]] auto run(int argc, char** argv) const -> int;

private:
    std::shared_ptr<application::use_cases::GenerateCvUseCase> use_case_;
};

} // namespace cvengine::presentation::cli
