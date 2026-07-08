#pragma once

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>
#include <string_view>

namespace cvengine::common {

[[nodiscard]] inline auto is_blank(std::string_view s) noexcept -> bool {
    return std::ranges::all_of(s, [](unsigned char c) {
        return std::isspace(c) != 0;
    });
}

[[nodiscard]] inline auto trim(std::string_view s) noexcept -> std::string_view {
    auto first = s.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string_view::npos) {
        return {};
    }
    auto last = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(first, last - first + 1);
}

// Returns true if the identifier is safe for use in file lookups:
// only alphanumeric, hyphens, underscores, and dots (no path separators
// or traversal sequences).
[[nodiscard]] inline auto is_safe_identifier(std::string_view s) noexcept -> bool {
    if (s.empty()) return false;
    return std::ranges::all_of(s, [](unsigned char c) {
        return std::isalnum(c) != 0 || c == '_' || c == '-' || c == '.';
    });
}

// Verifies that a resolved path is contained within the expected base
// directory (prevents path traversal attacks).
[[nodiscard]] inline auto is_path_within(
    const std::filesystem::path& child,
    const std::filesystem::path& base
) -> bool {
    std::error_code ec;
    auto canonical_base  = std::filesystem::canonical(base, ec);
    if (ec) return false;
    auto canonical_child = std::filesystem::canonical(child, ec);
    if (ec) return false;

    auto base_str  = canonical_base.string();
    auto child_str = canonical_child.string();
    if (child_str.size() < base_str.size()) return false;
    return child_str.compare(0, base_str.size(), base_str) == 0;
}

// Returns true if a filesystem path string is safe for shell usage
// (no shell metacharacters that could enable command injection).
[[nodiscard]] inline auto is_shell_safe(std::string_view s) noexcept -> bool {
    for (char c : s) {
        switch (c) {
            case ';': case '|': case '&': case '$':
            case '`': case '(': case ')': case '{':
            case '}': case '<': case '>': case '!':
            case '\n': case '\r':
                return false;
            default:
                break;
        }
    }
    return true;
}

} // namespace cvengine::common
