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

# ----- Librería del dominio (capa interna, sin deps externas) -----
add_library(cvengine-domain STATIC
    src/core/domain/period.cpp
    src/core/domain/contact.cpp
    src/core/domain/achievement.cpp
    src/core/domain/experience.cpp
    src/core/domain/education.cpp
    src/core/domain/certification.cpp
    src/core/domain/skill_group.cpp
    src/core/domain/professional_profile.cpp
    src/core/domain/cv.cpp
)

target_include_directories(cvengine-domain PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(cvengine-domain PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
    -Wnon-virtual-dtor -Wold-style-cast
    -Wcast-align -Wunused
    -Woverloaded-virtual -Wnull-dereference
    -Wdouble-promotion
)

# ----- Ejecutable principal -----
add_executable(cv-engine
    src/main.cpp
)

target_link_libraries(cv-engine PRIVATE
    cvengine-domain
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "13.0")
    target_link_libraries(cv-engine PRIVATE stdc++exp)
endif()

target_compile_options(cv-engine PRIVATE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion
)

message(STATUS "")
message(STATUS "==== cv-engine v${PROJECT_VERSION} ====")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "")
