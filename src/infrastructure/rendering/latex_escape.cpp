#include <cvengine/infrastructure/rendering/latex_escape.hpp>

namespace cvengine::infrastructure::rendering {

auto escape_latex(std::string_view input) -> std::string {
    std::string out;
    out.reserve(input.size() + (input.size() / 8));  // heurística: +12% espacio

    // Procesamos \ primero, luego el resto. Para evitar el "doble escape",
    // hacemos una sola pasada usando un switch.
    for (char c : input) {
        switch (c) {
            case '\\': out += "\\textbackslash{}";   break;
            case '&':  out += "\\&";                 break;
            case '%':  out += "\\%";                 break;
            case '$':  out += "\\$";                 break;
            case '#':  out += "\\#";                 break;
            case '_':  out += "\\_";                 break;
            case '{':  out += "\\{";                 break;
            case '}':  out += "\\}";                 break;
            case '~':  out += "\\textasciitilde{}";  break;
            case '^':  out += "\\textasciicircum{}"; break;
            default:   out += c;                     break;
        }
    }
    return out;
}

} // namespace cvengine::infrastructure::rendering
