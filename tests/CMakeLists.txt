cmake_minimum_required(VERSION 3.25)
project(search_engine1_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

file(GLOB TEST_SOURCES "tests.cpp")
add_executable(search_engine1_tests ${TEST_SOURCES})

target_link_libraries(
        search_engine1_tests
        PRIVATE
        search_engine_lib
        gtest_main
        gtest
        nlohmann_json::nlohmann_json
)
include(GoogleTest)
gtest_discover_tests(search_engine1_tests)