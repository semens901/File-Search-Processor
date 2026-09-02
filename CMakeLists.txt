cmake_minimum_required(VERSION 3.20)

project(fsp
    VERSION 0.1.0
    DESCRIPTION "File Search Processor - multithreaded file search utility for Linux"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------------------------------------------
# Compiler warnings
# ---------------------------------------------------------------------------

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------

file(GLOB_RECURSE FSP_SOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/fsp/*.cpp"
)

file(GLOB_RECURSE FSP_HEADER_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/fsp/*.h"
)

add_library(fsp_lib
    ${FSP_SOURCE_FILES}
    ${FSP_HEADER_FILES}
)

target_include_directories(fsp_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------

add_executable(fsp
    main.cpp
)

target_link_libraries(fsp
    PRIVATE
        fsp_lib
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

option(FSP_BUILD_TESTS "Build tests" ON)

if (FSP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()