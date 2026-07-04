#include <cvengine/core/domain/education.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Education::create(
    std::string_view degree,
    std::string_view institution,
    std::string_view location,
    std::string_view completion_date
) -> common::Result<Education> {
    auto deg_t = common::trim(degree);
    auto ins_t = common::trim(institution);
    auto loc_t = common::trim(location);
    auto date_t = common::trim(completion_date);

    if (auto err = common::first_error(
            common::require_non_empty(deg_t, "Education: degree required"),
            common::require_non_empty(ins_t, "Education: institution required"),
            common::require_non_empty(date_t, "Education: completion date required"))) {
        return std::unexpected{std::move(*err)};
    }

    return Education{
        std::string{deg_t},
        std::string{ins_t},
        std::string{loc_t},
        std::string{date_t}
    };
}

} // namespace cvengine::core::domain
