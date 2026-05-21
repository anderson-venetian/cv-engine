# cv-engine — Bitácora de decisiones arquitectónicas

> Proyecto: generador modular de CV en LaTeX
> Owner: Pedro Espinoza Trujillo (gvearde, +51927736128)
> Inicio: mayo 2026
> Arquitectura: hexagonal C++23 / CMake / CPM

---

## Fases arquitectónicas (commits feat:/chore:)

### Fase 1 — Bootstrap (commit 5ce2a46)
- C++23 + CMake 3.28 + GCC 16.1.0 MSYS2 MinGW-w64
- CPM como gestor de dependencias
- Preset `mingw-debug`
- Estructura de directorios para 4 capas

### Fase 2 — Capa de dominio (commit 942943c)
- 11 entidades puras con std::expected<T, Error> (C++23)
- Factory `create()` con validación en cada entidad
- Value semantics, sin dependencias externas
- Entidades: Cv, Contact, Period, Achievement, Experience, Education,
  Certification, SkillGroup, ProfessionalProfile, Theme, RenderedDocument

### Fase 3 — Ports (commit fc5d23c)
- 5 interfaces abstractas: IDataRepository, IThemeRepository,
  ITemplateEngine, IRenderer, ICompiler

### Fase 4 — Infraestructura: persistencia (commit 9a97692)
- JsonDataRepository (nlohmann/json 3.11.3)
- TomlThemeRepository (toml++ 3.4.0), aplana claves dotted a JSON anidado

### Fase 5 — Infraestructura: rendering (commit 85465fa)
- InjaTemplateEngine (inja 3.4.0)
- Sintaxis custom: (( )) expr, ## ## stmt, (# #) comentarios
- Helper set_nested() para aplanar TOML al contexto inja
- LatexRenderer orquesta plantilla + tema

### Fase 6 — Infraestructura: compilación (commit d6b114f)
- PdfLatexCompiler con 2 pasadas automáticas
- Cleanup configurable de archivos auxiliares
- Detección dinámica de pdflatex en PATH

### Fase 7 — Presentación (commit 125f3f9)
- CLI11 2.4.2 para argumentos
- GenerateCvUseCase orquesta repos + renderer + compiler
- main.cpp reducido a composition root puro
- Exit codes informativos para scripting (0/1/2/3/4)
- Modo silencioso por defecto: stdout = ruta del PDF

---

## Iteraciones estéticas

### Iteración A — Tipografía (commit 4fc851f, tipo: style)
- Default cambió de Latin Modern a Charter (XCharter)
- line_height 1.12 para densidad ejecutiva McKinsey/Harvard
- name_size_scale parametrizable
- font_package ahora editable desde TOML
- Análisis descartó Roboto (asociación Google/Material Design)
- Lato aceptable pero Source Sans Pro más neutral
- 5 temas disponibles: minimal (default), latin_modern,
  corporate, corporate_sans, executive_serif

### Iteración E — Links clicables (commit pendiente, tipo: feat)
- Nuevo value object ContactChannel con enum Type
  (WhatsApp, LinkedIn, GitHub, Website, Twitter, Other)
- Contact extendido con vector<ContactChannel> extra_channels
- URL pre-construida en el dominio según el tipo
- base.tex.j2 envuelve teléfono en \href{wa.me/...}
- email_raw (sin escapar) inyectado al contexto inja para \href{mailto:...}
- Activación de canales solo requiere editar JSON (cero recompilación)

---

## Decisiones arquitectónicas notables

### Por qué ContactChannel y no campos planos (iteración E)
- Escalable: agregar LinkedIn/GitHub no toca C++
- Es parte del dominio porque representa identidad de contacto
- Validación específica por tipo (E.164 para WhatsApp, URL con scheme para Website)

### Por qué Charter y no Times New Roman (iteración A)
- Charter mantiene legibilidad en pantalla mejor que Times
- Diseñada por Matthew Carter para impresoras láser de los 80s
- Robustez de trazos se traduce hoy en mejor escaneabilidad por ATS

### Por qué inja con sintaxis (( )) y no {{ }}
- {{ }} colisiona con LaTeX
- (( )) es libre y legible

### Por qué libstdc++exp en GCC ≥ 13
- std::print en MinGW requiere __open_terminal y __write_to_terminal
- Solo presentes en stdc++exp en GCC 13+

### mailto: y Chrome
- Chrome silencia mailto: por política de seguridad del visor
- Adobe Reader, Edge, Foxit, Outlook preview lo manejan correctamente
- Decisión: mantener el estándar correcto, no hacer workaround

---

## Lecciones de proceso

### Heredocs largos (>50 líneas)
- Riesgo de truncamiento silencioso
- MITIGACIÓN: verificación obligatoria con `wc -l` y `grep -c`
  tras cada heredoc grande
- Delimitadores únicos por archivo (no EOF repetido)

### Cuando algo "desaparece"
- output/ es derivado, regenerable en 15 segundos
- Auditoría con git status + ls antes de aplicar fixes
- Tres veces se ahorró tiempo aislando antes que adivinando

---

## Métricas finales

- Commits totales: 9
- Capas materializadas como librerías: 4
- Ports definidos: 5
- Adapters concretos: 5
- Use cases: 1
- Dependencias externas (vía CPM): 4
- Temas tipográficos: 5
- Canales de contacto modelados: 6 (1 activo: WhatsApp)
- Iteraciones estéticas pendientes: B (colores), C (geometría),
  D (separadores), F (layout)

---

## Roadmap futuro

### Iteraciones estéticas pendientes
- B: Paleta de colores
- C: Geometría (márgenes, hoja)
- D: Separadores (estilo, grosor)
- F: Layout (orden de secciones)

### Posibles iteraciones funcionales
- Activar canales adicionales: LinkedIn, GitHub, Website
- --keep-tex flag en CLI
- Soporte para múltiples idiomas (inglés)
- Modo --list-themes
- Plantilla alternativa (academic, modern, classic)

---

## Stack técnico

- C++23 (GCC 16.1.0)
- CMake 3.28 + CPM
- MSYS2 MinGW-w64
- MiKTeX para LaTeX (pdflatex)
- nlohmann/json 3.11.3
- toml++ 3.4.0
- inja 3.4.0
- CLI11 2.4.2

---

## Cómo retomar

```bash
cd /c/Proyectos/cv-engine
git log --oneline | head -10
./build/debug/bin/cv-engine.exe -v
```

Estado actual: `git log` muestra 9 commits.
PDF default: Charter serif con links clicables en encabezado.
