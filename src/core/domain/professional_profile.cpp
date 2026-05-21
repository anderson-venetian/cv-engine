#include <cvengine/core/domain/professional_profile.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto ProfessionalProfile::create(std::string_view text) -> common::Result<ProfessionalProfile> {
    auto trimmed = common::trim(text);
    if (trimmed.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "ProfessionalProfile cannot be empty"
        );
    }
    if (trimmed.size() < kMinLength) {
        return common::make_error(
            common::ErrorCode::InvalidLength,
            "ProfessionalProfile too short (minimum 50 chars)"
        );
    }
    if (trimmed.size() > kMaxLength) {
        return common::make_error(
            common::ErrorCode::InvalidLength,
            "ProfessionalProfile too long (maximum 1500 chars)"
        );
    }
    return ProfessionalProfile{std::string{trimmed}};
}

} // namespace cvengine::core::domain
