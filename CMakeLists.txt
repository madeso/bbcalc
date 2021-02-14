cmake_minimum_required(VERSION 3.12.4)
project(bbcalc CXX)

set(CMAKE_CXX_EXTENSIONS OFF)

##############################################################################
## project setup
include(tools/cmake/StandardProjectSettings.cmake)
include(tools/cmake/CompilerWarnings.cmake)
include(tools/cmake/Sanitizers.cmake)

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
    src/calc/calc.cc src/calc/calc.h
    src/calc/str.cc src/calc/str.h
    src/calc/output.cc src/calc/output.h
    src/calc/errorhandler.cc src/calc/errorhandler.h
    src/calc/token.cc src/calc/token.h
    src/calc/input.h
    src/calc/ints.cc src/calc/ints.h
    src/calc/lexer.cc src/calc/lexer.h
    src/calc/ast.cc src/calc/ast.h
    src/calc/parser.cc src/calc/parser.h
    src/calc/binary.cc src/calc/binary.h
)
target_include_directories(calculator
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(calculator
    PRIVATE
    project_options
    project_warnings
)

##############################################################################
## calc commandline

add_executable(bbcalc
    examples/cli.cc
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

include(CTest)
catch_discover_tests(tests)
