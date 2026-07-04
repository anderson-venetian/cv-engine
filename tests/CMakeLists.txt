add_executable(cvengine-tests
    unit/test_period.cpp
    unit/test_contact_channel.cpp
    unit/test_contact.cpp
    unit/test_cv.cpp
    unit/test_achievement.cpp
    unit/test_certification.cpp
    unit/test_education.cpp
    unit/test_experience.cpp
    unit/test_professional_profile.cpp
    unit/test_rendered_document.cpp
    unit/test_skill_group.cpp
    unit/test_theme.cpp
    unit/test_latex_escape.cpp
    unit/test_validation.cpp
)

target_link_libraries(cvengine-tests
    PRIVATE
        cvengine-core
        cvengine-application
        cvengine-infrastructure
        Catch2::Catch2WithMain
)

target_include_directories(cvengine-tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Activar estándar C++23 para el ejecutable de pruebas
set_target_properties(cvengine-tests PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

add_test(NAME cvengine-unit-tests COMMAND cvengine-tests)
