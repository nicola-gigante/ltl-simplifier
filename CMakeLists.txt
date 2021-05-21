cmake_minimum_required(VERSION 3.18)

project(Simplifier LANGUAGES C CXX)

add_library(owl SHARED IMPORTED)
set_target_properties(owl PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/external/owl/clib/libowl.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/external/owl/clib"
)

add_executable(simplifier src/main.cpp)
target_link_libraries(simplifier PRIVATE owl)