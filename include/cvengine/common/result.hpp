#pragma once

#include <expected>
#include <cvengine/common/error.hpp>

namespace cvengine::common {

template <typename T>
using Result = std::expected<T, Error>;

[[nodiscard]] inline auto make_error(ErrorCode code, std::string_view msg)
    -> std::unexpected<Error> {
    return std::unexpected<Error>{Error{code, msg}};
}

} // namespace cvengine::common
