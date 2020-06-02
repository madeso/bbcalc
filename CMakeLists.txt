cmake_minimum_required(VERSION 3.12.4)
project(bbcalc CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################################
## project setup
include(cmake/StandardProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
enable_sanitizers(project_options)

add_library(project_warnings INTERFACE)
set_project_warnings(project_warnings)


##############################################################################
## dependencies
add_subdirectory(external/catchy)


##############################################################################
## calc library

add_library(calculator STATIC
    calc/calc.cc calc/calc.h
    calc/str.cc calc/str.h
    calc/output.cc calc/output.h
)
target_include_directories(calculator
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(calculator
    PRIVATE
    project_options
    project_warnings
)

##############################################################################
## calc commandline

add_executable(bbcalc
    cli/main.cc
)
target_link_libraries(bbcalc
    PUBLIC
    calculator
    PRIVATE
    project_options
    project_warnings
)


##############################################################################
## calc (unit) tests

add_executable(tests
    tests/main.cc
    tests/test_calc.cc
)
target_link_libraries(
    tests
    PUBLIC
    calculator
    catchy
    ${CMAKE_THREAD_LIBS_INIT}
    PRIVATE
    project_options
    project_warnings
)

