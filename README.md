# cv-engine

Generador modular de CV en LaTeX con configuración dinámica de datos y temas.

## Stack

- **C++23** (ISO)
- **GCC 16.1.0** (MSYS2 MinGW-w64)
- **CMake 3.28+** con presets
- **Ninja** como generador
- **MiKTeX** para compilación PDF

## Build

```bash
cmake --preset mingw-debug
cmake --build build/debug
./build/debug/bin/cv-engine.exe
```

## Arquitectura

Hexagonal (Ports & Adapters). Ver `docs/ARCHITECTURE.md`.
