cmake_minimum_required(VERSION 3.24)

project(advanced_algorithms LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
        mimalloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG v2.2.3
        GIT_SHALLOW ON
)

add_compile_definitions(MI_OVERRIDE=ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

FetchContent_MakeAvailable(mimalloc)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND MSVC)
    set(OpenMP_RUNTIME_MSVC llvm)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-march=native)
endif()

find_package(OpenMP REQUIRED)

add_subdirectory(1)
add_subdirectory(2)
