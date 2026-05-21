#pragma once

#include <algorithm>
#include <cctype>
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

} // namespace cvengine::common
