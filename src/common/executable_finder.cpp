#include <cvengine/common/executable_finder.hpp>

#include <cstdlib>
#include <sstream>
#include <vector>

namespace cvengine::common {

namespace fs = std::filesystem;

namespace {

#ifdef _WIN32
constexpr char kPathSeparator = ';';
#else
constexpr char kPathSeparator = ':';
#endif

auto split_path(const std::string& path_env) -> std::vector<std::string> {
    std::vector<std::string> parts;
    std::stringstream ss{path_env};
    std::string item;
    while (std::getline(ss, item, kPathSeparator)) {
        if (!item.empty()) parts.push_back(item);
    }
    return parts;
}

auto try_with_extension(const fs::path& candidate) -> std::optional<fs::path> {
    if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
        return candidate;
    }
#ifdef _WIN32
    auto with_exe = candidate;
    with_exe += ".exe";
    if (fs::exists(with_exe) && fs::is_regular_file(with_exe)) {
        return with_exe;
    }
#endif
    return std::nullopt;
}

} // anonymous namespace

auto find_executable(std::string_view name) -> std::optional<fs::path> {
    fs::path requested{name};

    if (requested.is_absolute() || requested.has_parent_path()) {
        return try_with_extension(requested);
    }

    const char* path_env = std::getenv("PATH");
    if (path_env == nullptr) {
        return std::nullopt;
    }

    for (const auto& dir : split_path(path_env)) {
        auto candidate = fs::path{dir} / requested;
        if (auto found = try_with_extension(candidate)) {
            return found;
        }
    }

    return std::nullopt;
}

} // namespace cvengine::common
