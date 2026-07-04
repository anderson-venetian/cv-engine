#include <cvengine/core/domain/achievement.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Achievement::create(std::string_view text) -> common::Result<Achievement> {
    auto trimmed = common::trim(text);
    if (auto err = common::first_error(
            common::require_non_empty(trimmed, "Achievement text cannot be empty"),
            common::require_max_length(trimmed, kMaxLength, "Achievement exceeds maximum length"))) {
        return std::unexpected{std::move(*err)};
    }
    return Achievement{std::string{trimmed}};
}

} // namespace cvengine::core::domain
