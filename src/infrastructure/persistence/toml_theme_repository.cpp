#include <cvengine/infrastructure/persistence/toml_theme_repository.hpp>

#include <sstream>
#include <toml++/toml.hpp>

namespace cvengine::infrastructure::persistence {

namespace {

using common::ErrorCode;
using common::make_error;
namespace dom = core::domain;

// Aplana una tabla TOML jerárquica a claves dotted.
// [colors] primary = "#x"  →  "colors.primary" : "#x"
auto flatten(const toml::table& table,
             const std::string& prefix,
             dom::Theme::PropertyMap& out) -> void {
    for (const auto& [key, node] : table) {
        std::string full_key = prefix.empty()
            ? std::string{key.str()}
            : prefix + "." + std::string{key.str()};

        if (node.is_table()) {
            flatten(*node.as_table(), full_key, out);
        } else if (node.is_string()) {
            out[full_key] = node.as_string()->get();
        } else if (node.is_integer()) {
            out[full_key] = std::to_string(node.as_integer()->get());
        } else if (node.is_floating_point()) {
            out[full_key] = std::to_string(node.as_floating_point()->get());
        } else if (node.is_boolean()) {
            out[full_key] = node.as_boolean()->get() ? "true" : "false";
        }
        // Arrays y otros tipos se ignoran en esta primera versión.
    }
}

} // anonymous namespace

TomlThemeRepository::TomlThemeRepository(std::filesystem::path base_directory)
    : base_directory_{std::move(base_directory)} {}

auto TomlThemeRepository::load(std::string_view name) const
    -> common::Result<dom::Theme> {

    auto path = base_directory_ / (std::string{name} + ".toml");

    if (!std::filesystem::exists(path)) {
        std::ostringstream msg;
        msg << "Theme file not found: " << path.string();
        return make_error(ErrorCode::FileNotFound, msg.str());
    }

    toml::table table;
    try {
        table = toml::parse_file(path.string());
    } catch (const toml::parse_error& e) {
        std::ostringstream msg;
        msg << "TOML parse error: " << e.description();
        return make_error(ErrorCode::ParseError, msg.str());
    }

    dom::Theme::PropertyMap properties;
    flatten(table, "", properties);

    // El nombre lógico viene del filename, pero si el TOML tiene [meta] name = "...",
    // ese tiene prioridad.
    std::string theme_name{name};
    if (auto meta_name_it = properties.find("meta.name"); meta_name_it != properties.end()) {
        theme_name = meta_name_it->second;
    }

    return dom::Theme::create(theme_name, std::move(properties));
}

} // namespace cvengine::infrastructure::persistence
