#include <cvengine/core/domain/certification.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Certification::create(
    std::string_view name,
    std::string_view issuer,
    std::string_view date
) -> common::Result<Certification> {
    auto name_t = common::trim(name);
    auto issuer_t = common::trim(issuer);
    auto date_t = common::trim(date);

    if (auto err = common::first_error(
            common::require_non_empty(name_t, "Certification: name required"),
            common::require_non_empty(issuer_t, "Certification: issuer required"),
            common::require_non_empty(date_t, "Certification: date required"))) {
        return std::unexpected{std::move(*err)};
    }

    return Certification{
        std::string{name_t},
        std::string{issuer_t},
        std::string{date_t}
    };
}

} // namespace cvengine::core::domain
