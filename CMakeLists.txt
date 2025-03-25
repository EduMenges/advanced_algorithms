cmake_minimum_required(VERSION 3.24)

project(advanced_algorithms LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

add_compile_definitions(MI_OVERRIDE=ON)

include(ExternalProject)

ExternalProject_Add(
        mimalloc_build
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/mimalloc
        CMAKE_ARGS
        -DMI_OVERRIDE=ON
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}
        -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
)

ExternalProject_Get_Property(mimalloc_build ${INSTALL_DIR})
set(mimalloc_DIR ${INSTALL_DIR}/lib/cmake)

find_package(mimalloc CONFIG)

include(FetchContent)

FetchContent_Declare(
        mimalloc
        GIT_REPOSITORY https://github.com/microsoft/mimalloc.git
        GIT_TAG v2.2.2
        GIT_SHALLOW ON
)

add_compile_definitions(MI_OVERRIDE=ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

FetchContent_MakeAvailable(mimalloc)

link_libraries(mimalloc)

add_subdirectory(1)
