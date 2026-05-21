#include <cvengine/core/domain/rendered_document.hpp>

namespace cvengine::core::domain {

auto RenderedDocument::create(Format format, std::string content)
    -> common::Result<RenderedDocument> {
    if (content.empty()) {
        return common::make_error(
            common::ErrorCode::EmptyRequiredField,
            "RenderedDocument: content cannot be empty"
        );
    }
    return RenderedDocument{format, std::move(content)};
}

auto RenderedDocument::format_extension(Format f) noexcept -> std::string_view {
    switch (f) {
        case Format::Latex:    return "tex";
        case Format::Html:     return "html";
        case Format::Markdown: return "md";
    }
    return "txt";
}

} // namespace cvengine::core::domain
