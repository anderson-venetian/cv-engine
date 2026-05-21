#include <cvengine/infrastructure/compilation/pdflatex_compiler.hpp>
#include <cvengine/common/executable_finder.hpp>

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <array>

namespace cvengine::infrastructure::compilation {

namespace fs = std::filesystem;
namespace dom = core::domain;
using common::ErrorCode;
using common::make_error;

namespace {

auto build_command(
    const std::string& pdflatex_path,
    const fs::path& working_dir,
    const std::string& tex_filename
) -> std::string {
    std::ostringstream cmd;
#ifdef _WIN32
    cmd << "\"";
#endif
    cmd << "\"" << pdflatex_path << "\""
        << " -interaction=nonstopmode"
        << " -halt-on-error"
        << " -output-directory=\"" << working_dir.string() << "\""
        << " \"" << (working_dir / tex_filename).string() << "\"";
#ifdef _WIN32
    cmd << "\"";
#endif
    cmd << " > NUL 2>&1";
    return cmd.str();
}

auto write_tex_file(
    const fs::path& path,
    const std::string& content
) -> common::Result<void> {
    std::ofstream out{path, std::ios::binary};
    if (!out) {
        std::ostringstream msg;
        msg << "Cannot write tex file: " << path.string();
        return std::unexpected{common::Error{ErrorCode::IoError, msg.str()}};
    }
    out << content;
    if (!out) {
        return std::unexpected{common::Error{ErrorCode::IoError, "Write failed"}};
    }
    return {};
}

auto cleanup_aux_files(const fs::path& working_dir, const std::string& base) -> void {
    constexpr std::array kAuxExtensions{
        ".aux", ".log", ".out", ".toc", ".synctex.gz",
        ".fls", ".fdb_latexmk", ".bbl", ".blg"
    };
    for (const auto& ext : kAuxExtensions) {
        auto p = working_dir / (base + ext);
        std::error_code ec;
        fs::remove(p, ec);
    }
}

} // anonymous namespace

PdfLatexCompiler::PdfLatexCompiler(
    std::string executable_path,
    std::string base_filename,
    bool cleanup_auxiliaries,
    int pass_count
)
    : executable_path_{std::move(executable_path)}
    , base_filename_{std::move(base_filename)}
    , cleanup_auxiliaries_{cleanup_auxiliaries}
    , pass_count_{pass_count < 1 ? 1 : pass_count} {}

auto PdfLatexCompiler::compile(
    const dom::RenderedDocument& document,
    const fs::path& output_directory
) const -> common::Result<fs::path> {

    if (document.format() != dom::RenderedDocument::Format::Latex) {
        return make_error(
            ErrorCode::CompilationError,
            "PdfLatexCompiler only accepts Latex documents"
        );
    }

    std::string resolved_path = executable_path_;
    if (resolved_path.empty()) {
        auto found = common::find_executable("pdflatex");
        if (!found) {
            return make_error(
                ErrorCode::CompilationError,
                "pdflatex not found in PATH. Install MiKTeX or TeX Live."
            );
        }
        resolved_path = found->string();
    } else {
        if (!fs::exists(resolved_path)) {
            std::ostringstream msg;
            msg << "pdflatex executable does not exist: " << resolved_path;
            return make_error(ErrorCode::CompilationError, msg.str());
        }
    }

    std::error_code ec;
    fs::create_directories(output_directory, ec);
    if (ec) {
        std::ostringstream msg;
        msg << "Cannot create output directory: " << ec.message();
        return make_error(ErrorCode::IoError, msg.str());
    }

    auto tex_filename = base_filename_ + ".tex";
    auto tex_path = output_directory / tex_filename;
    if (auto r = write_tex_file(tex_path, document.content()); !r) {
        return std::unexpected{r.error()};
    }

    auto cmd = build_command(resolved_path, output_directory, tex_filename);
    for (int pass = 1; pass <= pass_count_; ++pass) {
        int rc = std::system(cmd.c_str());
        if (rc != 0) {
            std::ostringstream msg;
            msg << "pdflatex failed on pass " << pass
                << " (exit code " << rc << "). "
                << "See " << (output_directory / (base_filename_ + ".log")).string()
                << " for details.";
            return make_error(ErrorCode::CompilationError, msg.str());
        }
    }

    auto pdf_path = output_directory / (base_filename_ + ".pdf");
    if (!fs::exists(pdf_path)) {
        return make_error(
            ErrorCode::CompilationError,
            "pdflatex completed but no PDF was produced. Check the .log file."
        );
    }

    if (cleanup_auxiliaries_) {
        cleanup_aux_files(output_directory, base_filename_);
    }

    return fs::absolute(pdf_path);
}

auto PdfLatexCompiler::accepted_format() const noexcept
    -> dom::RenderedDocument::Format {
    return dom::RenderedDocument::Format::Latex;
}

} // namespace cvengine::infrastructure::compilation
