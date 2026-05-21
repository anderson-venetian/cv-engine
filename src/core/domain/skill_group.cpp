#include <cvengine/core/domain/skill_group.hpp>
#include <cvengine/common/validation.hpp>

namespace cvengine::core::domain {

auto SkillGroup::create(
    std::string_view label,
    std::string_view content
) -> common::Result<SkillGroup> {
    auto label_t = common::trim(label);
    auto content_t = common::trim(content);

    if (label_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField, "SkillGroup: label required");
    }
    if (content_t.empty()) {
        return common::make_error(common::ErrorCode::EmptyRequiredField, "SkillGroup: content required");
    }

    return SkillGroup{std::string{label_t}, std::string{content_t}};
}

} // namespace cvengine::core::domain
