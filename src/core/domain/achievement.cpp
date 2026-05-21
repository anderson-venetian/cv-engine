#include <cvengine/core/domain/achievement.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Achievement::create(std::string_view text) -> common::Result<Achievement> {
    auto trimmed = common::trim(text);
    if (trimmed.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Achievement text cannot be empty"
        );
    }
    if (trimmed.size() > kMaxLength) {
        return common::make_error(
            common::ErrorCode::InvalidLength,
            "Achievement exceeds maximum length"
        );
    }
    return Achievement{std::string{trimmed}};
}

} // namespace cvengine::core::domain
