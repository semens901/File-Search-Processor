cmake_minimum_required(VERSION 3.20)

project(fsp
    VERSION 0.1.0
    DESCRIPTION "File Search Processor - multithreaded file search utility for Linux"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    bs_thread_pool
    GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
    GIT_TAG v5.1.0
)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)

FetchContent_MakeAvailable(bs_thread_pool spdlog)

add_library(BS::thread_pool INTERFACE IMPORTED)
set_target_properties(BS::thread_pool PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${bs_thread_pool_SOURCE_DIR}/include"
)

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

target_link_libraries(fsp_lib
    PUBLIC
        BS::thread_pool
        spdlog
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