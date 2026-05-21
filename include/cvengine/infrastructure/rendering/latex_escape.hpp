#pragma once

#include <string>
#include <string_view>

namespace cvengine::infrastructure::rendering {

// Escapa caracteres especiales LaTeX en texto plano.
// Reglas:
//   & → \&     % → \%     $ → \$     # → \#
//   _ → \_     { → \{     } → \}     ~ → \textasciitilde{}
//   ^ → \textasciicircum{}            \ → \textbackslash{}
//
// El orden de procesamiento importa: \ se reemplaza primero
// para no doblemente escapar las secuencias generadas por el resto.
[[nodiscard]] auto escape_latex(std::string_view input) -> std::string;

} // namespace cvengine::infrastructure::rendering
