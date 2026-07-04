#include <cvengine/core/domain/professional_profile.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto ProfessionalProfile::create(std::string_view text) -> common::Result<ProfessionalProfile> {
    auto trimmed = common::trim(text);
    if (auto err = common::first_error(
            common::require_non_empty(trimmed, "ProfessionalProfile cannot be empty"),
            common::require_min_length(trimmed, kMinLength,
                "ProfessionalProfile too short (minimum 50 chars)"),
            common::require_max_length(trimmed, kMaxLength,
                "ProfessionalProfile too long (maximum 1500 chars)"))) {
        return std::unexpected{std::move(*err)};
    }
    return ProfessionalProfile{std::string{trimmed}};
}

} // namespace cvengine::core::domain
