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
add_subdirectory(src)

##############################################################################
## calc commandline

add_subdirectory(examples)

##############################################################################
## calc (unit) tests

add_subdirectory(tests)