#include <cvengine/core/domain/theme.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto Theme::create(
    std::string_view name,
    PropertyMap properties
) -> common::Result<Theme> {
    auto name_t = common::trim(name);
    if (auto err = common::require_non_empty(name_t, "Theme: name is required")) {
        return std::unexpected{std::move(*err)};
    }
    if (properties.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "Theme: at least one property is required"
        );
    }
    return Theme{std::string{name_t}, std::move(properties)};
}

auto Theme::get(std::string_view key, std::string_view fallback) const -> std::string {
    auto it = properties_.find(std::string{key});
    if (it == properties_.end()) {
        return std::string{fallback};
    }
    return it->second;
}

auto Theme::has(std::string_view key) const noexcept -> bool {
    return properties_.contains(std::string{key});
}

} // namespace cvengine::core::domain
