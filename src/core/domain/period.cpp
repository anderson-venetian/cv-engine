#include <cvengine/core/domain/period.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Period::create(std::string_view text) -> common::Result<Period> {
    auto trimmed = common::trim(text);
    if (trimmed.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Period text cannot be empty"
        );
    }
    if (trimmed.size() > 100) {
        return common::make_error(
            common::ErrorCode::InvalidLength,
            "Period text exceeds 100 characters"
        );
    }
    return Period{std::string{trimmed}};
}

} // namespace cvengine::core::domain
