cmake_minimum_required(VERSION 3.28)

project(cv-engine
    VERSION 2.0.0
    DESCRIPTION "Generador modular de CV en LaTeX"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ===== Dependencias externas vía CPM =====
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

CPMAddPackage(
    NAME nlohmann_json
    VERSION 3.11.3
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    OPTIONS "JSON_BuildTests OFF"
)

CPMAddPackage(
    NAME tomlplusplus
    GITHUB_REPOSITORY marzer/tomlplusplus
    VERSION 3.4.0
)

CPMAddPackage(
    NAME inja
    GITHUB_REPOSITORY pantor/inja
    VERSION 3.4.0
    OPTIONS
        "INJA_USE_EMBEDDED_JSON OFF"
        "INJA_BUILD_TESTS OFF"
        "BUILD_BENCHMARK OFF"
)

if(TARGET inja)
    get_target_property(_inja_inc inja INTERFACE_INCLUDE_DIRECTORIES)
    if(_inja_inc)
        set_target_properties(inja PROPERTIES
            INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${_inja_inc}")
    endif()
endif()

CPMAddPackage(
    NAME CLI11
    GITHUB_REPOSITORY CLIUtils/CLI11
    VERSION 2.4.2
    OPTIONS
        "CLI11_BUILD_TESTS OFF"
        "CLI11_BUILD_EXAMPLES OFF"
        "CLI11_BUILD_DOCS OFF"
)

CPMAddPackage(
    NAME Catch2
    GITHUB_REPOSITORY catchorg/Catch2
    VERSION 3.4.0
)


# ===== Capa core (dominio + ports + common) =====
add_library(cvengine-core STATIC
    src/core/domain/period.cpp
    src/core/domain/contact.cpp
    src/core/domain/achievement.cpp
    src/core/domain/experience.cpp
    src/core/domain/education.cpp
    src/core/domain/certification.cpp
    src/core/domain/skill_group.cpp
    src/core/domain/professional_profile.cpp
    src/core/domain/contact_channel.cpp
    src/core/domain/theme.cpp
    src/core/domain/rendered_document.cpp
    src/core/domain/cv.cpp
    src/common/executable_finder.cpp
)

target_include_directories(cvengine-core PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(cvengine-core PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
    -Wnon-virtual-dtor -Wold-style-cast
    -Wcast-align -Wunused
    -Woverloaded-virtual -Wnull-dereference
    -Wdouble-promotion
)

# ===== Capa application (use cases) =====
add_library(cvengine-application STATIC
    src/application/use_cases/generate_cv_use_case.cpp
)

target_include_directories(cvengine-application PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cvengine-application
    PUBLIC cvengine-core
)

target_compile_options(cvengine-application PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
)

# ===== Capa infrastructure =====
add_library(cvengine-infrastructure STATIC
    src/infrastructure/persistence/json_data_repository.cpp
    src/infrastructure/persistence/toml_theme_repository.cpp
    src/infrastructure/rendering/inja_template_engine.cpp
    src/infrastructure/rendering/latex_renderer.cpp
    src/infrastructure/rendering/latex_escape.cpp
    src/infrastructure/compilation/pdflatex_compiler.cpp
)

target_include_directories(cvengine-infrastructure PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cvengine-infrastructure
    PUBLIC
        cvengine-core
    PRIVATE
        nlohmann_json::nlohmann_json
        tomlplusplus::tomlplusplus
        inja
)

target_compile_options(cvengine-infrastructure PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
)

# ===== Capa presentation (CLI) =====
add_library(cvengine-presentation STATIC
    src/presentation/cli/cli_application.cpp
)

target_include_directories(cvengine-presentation PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(cvengine-presentation
    PUBLIC
        cvengine-core
        cvengine-application
    PRIVATE
        CLI11::CLI11
)

target_compile_options(cvengine-presentation PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
)

# ===== Ejecutable principal (composition root) =====
add_executable(cv-engine
    src/main.cpp
)

target_link_libraries(cv-engine PRIVATE
    cvengine-core
    cvengine-application
    cvengine-infrastructure
    cvengine-presentation
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13.0")
    target_link_libraries(cv-engine PRIVATE stdc++exp)
endif()

target_compile_options(cv-engine PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
)

# ===== Pruebas unitarias e integración =====
option(BUILD_TESTING "Construir las pruebas unitarias" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

message(STATUS "")
message(STATUS "==== cv-engine v${PROJECT_VERSION} ====")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
