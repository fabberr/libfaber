#
# Top-level CMakeLists.txt
#
cmake_minimum_required(VERSION 3.21)

project(faber VERSION 1.0.0 LANGUAGES CXX)

# Adding targets (library modules)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/io")
