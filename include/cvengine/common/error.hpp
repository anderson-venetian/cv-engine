#pragma once

#include <string>
#include <string_view>

namespace cvengine::common {

enum class ErrorCode {
    // Dominio
    EmptyRequiredField,
    InvalidLength,
    InvalidFormat,
    InvalidDateRange,
    DuplicateEntry,

    // Infraestructura (reservados para fases futuras)
    FileNotFound,
    ParseError,
    RenderError,
    CompilationError,
    IoError,

    Unknown
};

struct Error {
    ErrorCode code;
    std::string message;

    Error(ErrorCode c, std::string_view msg)
        : code{c}, message{msg} {}
};

[[nodiscard]] constexpr auto to_string(ErrorCode c) noexcept -> std::string_view {
    switch (c) {
        case ErrorCode::EmptyRequiredField: return "EmptyRequiredField";
        case ErrorCode::InvalidLength:      return "InvalidLength";
        case ErrorCode::InvalidFormat:      return "InvalidFormat";
        case ErrorCode::InvalidDateRange:   return "InvalidDateRange";
        case ErrorCode::DuplicateEntry:     return "DuplicateEntry";
        case ErrorCode::FileNotFound:       return "FileNotFound";
        case ErrorCode::ParseError:         return "ParseError";
        case ErrorCode::RenderError:        return "RenderError";
        case ErrorCode::CompilationError:   return "CompilationError";
        case ErrorCode::IoError:            return "IoError";
        case ErrorCode::Unknown:            return "Unknown";
    }
    return "Unknown";
}

} // namespace cvengine::common
