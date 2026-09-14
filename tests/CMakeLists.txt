include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(Catch2)

# ---------------------------------------------------------------------------
# Unit tests
# ---------------------------------------------------------------------------

file(GLOB FSP_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/unit/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/utils/*.cpp"
)

add_executable(fsp_unit_tests
    ${FSP_TEST_SOURCES}
)

target_link_libraries(fsp_unit_tests
    PRIVATE
        fsp_lib
        Catch2::Catch2WithMain
)

# ---------------------------------------------------------------------------
# CTest
# ---------------------------------------------------------------------------

include(CTest)

include(Catch)

catch_discover_tests(fsp_unit_tests)