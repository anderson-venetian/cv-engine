#include <cvengine/application/use_cases/generate_cv_use_case.hpp>

#include <filesystem>

namespace cvengine::application::use_cases {

namespace fs = std::filesystem;

GenerateCvUseCase::GenerateCvUseCase(
    std::shared_ptr<core::ports::IDataRepository>  data_repo,
    std::shared_ptr<core::ports::IThemeRepository> theme_repo,
    std::shared_ptr<core::ports::IRenderer>        renderer,
    std::shared_ptr<core::ports::ICompiler>        compiler
)
    : data_repo_{std::move(data_repo)}
    , theme_repo_{std::move(theme_repo)}
    , renderer_{std::move(renderer)}
    , compiler_{std::move(compiler)} {}

auto GenerateCvUseCase::execute(const GenerateCvRequest& request) const
    -> common::Result<GenerateCvResponse> {

    // 1. Cargar CV
    auto cv_r = data_repo_->load(request.data_identifier);
    if (!cv_r) return std::unexpected{cv_r.error()};

    // 2. Cargar tema
    auto theme_r = theme_repo_->load(request.theme_name);
    if (!theme_r) return std::unexpected{theme_r.error()};

    // 3. Renderizar a documento intermedio
    auto doc_r = renderer_->render(*cv_r, *theme_r);
    if (!doc_r) return std::unexpected{doc_r.error()};

    // 4. Compilar a PDF
    auto pdf_r = compiler_->compile(*doc_r, request.output_dir);
    if (!pdf_r) return std::unexpected{pdf_r.error()};

    // 5. Empaquetar respuesta
    std::error_code ec;
    auto size = fs::file_size(*pdf_r, ec);
    if (ec) size = 0;

    return GenerateCvResponse{
        .pdf_path  = *std::move(pdf_r),
        .pdf_bytes = size
    };
}

} // namespace cvengine::application::use_cases
