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

    if (name_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField, "Certification: name required");
    }
    if (issuer_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField, "Certification: issuer required");
    }
    if (date_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField, "Certification: date required");
    }

    return Certification{
        std::string{name_t},
        std::string{issuer_t},
        std::string{date_t}
    };
}

} // namespace cvengine::core::domain
