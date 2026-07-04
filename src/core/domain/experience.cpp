#include <cvengine/core/domain/experience.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Experience::create(
    std::string_view position,
    std::string_view organization,
    std::string_view location,
    Period period,
    std::vector<Achievement> achievements
) -> common::Result<Experience> {
    auto pos_t = common::trim(position);
    auto org_t = common::trim(organization);
    auto loc_t = common::trim(location);

    if (auto err = common::first_error(
            common::require_non_empty(pos_t, "Experience: position required"),
            common::require_non_empty(org_t, "Experience: organization required"),
            common::require_non_empty(loc_t, "Experience: location required"))) {
        return std::unexpected{std::move(*err)};
    }
    if (achievements.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Experience: at least one achievement required"
        );
    }

    return Experience{
        std::string{pos_t},
        std::string{org_t},
        std::string{loc_t},
        std::move(period),
        std::move(achievements)
    };
}

} // namespace cvengine::core::domain
