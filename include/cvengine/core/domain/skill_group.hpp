#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

// Una categoría de habilidades con su contenido en texto libre.
// Ej: { label: "Marcos y métricas", content: "ITIL 4, SLA, CSAT..." }
class SkillGroup {
public:
    [[nodiscard]] static auto create(
        std::string_view label,
        std::string_view content
    ) -> common::Result<SkillGroup>;

    [[nodiscard]] auto label()   const noexcept -> const std::string& { return label_; }
    [[nodiscard]] auto content() const noexcept -> const std::string& { return content_; }

    [[nodiscard]] auto operator==(const SkillGroup&) const -> bool = default;

private:
    SkillGroup(std::string label, std::string content)
        : label_{std::move(label)}, content_{std::move(content)} {}

    std::string label_;
    std::string content_;
};

} // namespace cvengine::core::domain
