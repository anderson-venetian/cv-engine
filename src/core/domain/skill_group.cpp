#include <cvengine/core/domain/skill_group.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto SkillGroup::create(
    std::string_view label,
    std::string_view content
) -> common::Result<SkillGroup> {
    auto label_t = common::trim(label);
    auto content_t = common::trim(content);

    if (auto err = common::first_error(
            common::require_non_empty(label_t, "SkillGroup: label required"),
            common::require_non_empty(content_t, "SkillGroup: content required"))) {
        return std::unexpected{std::move(*err)};
    }

    return SkillGroup{std::string{label_t}, std::string{content_t}};
}

} // namespace cvengine::core::domain
