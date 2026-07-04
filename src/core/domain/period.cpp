#include <cvengine/core/domain/period.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Period::create(std::string_view text) -> common::Result<Period> {
    auto trimmed = common::trim(text);
    if (auto err = common::first_error(
            common::require_non_empty(trimmed, "Period text cannot be empty"),
            common::require_max_length(trimmed, 100, "Period text exceeds 100 characters"))) {
        return std::unexpected{std::move(*err)};
    }
    return Period{std::string{trimmed}};
}

} // namespace cvengine::core::domain
