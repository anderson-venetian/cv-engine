#pragma once

#include <string>
#include <string_view>
#include <cvengine/common/result.hpp>

namespace cvengine::core::domain {

class ProfessionalProfile {
public:
    static constexpr std::size_t kMinLength = 50;
    static constexpr std::size_t kMaxLength = 1500;

    [[nodiscard]] static auto create(std::string_view text) -> common::Result<ProfessionalProfile>;

    [[nodiscard]] auto text() const noexcept -> const std::string& { return text_; }

    [[nodiscard]] auto operator==(const ProfessionalProfile&) const -> bool = default;

private:
    explicit ProfessionalProfile(std::string text) : text_{std::move(text)} {}
    std::string text_;
};

} // namespace cvengine::core::domain
