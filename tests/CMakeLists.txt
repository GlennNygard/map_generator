cmake_minimum_required(VERSION "3.27.0")
set(CMAKE_CXX_STANDARD 20)

# TEST PROJECT.
project(tests)

# Fetch and make Catch2 available.
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.7.0 # or a later release
    )

FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# Add the test source files.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add the test executable.
add_executable(tests ${TEST_SOURCES})

target_compile_definitions(tests PUBLIC UNIT_TEST)

if(PRODUCTION_BUILD)
    # Setup the asset path macro to be in the root folder of the executable.
    target_compile_definitions(tests PUBLIC RESOURCE_PATH_TEST="./res/")
else()
    # This is useful to get an ASSETS_PATH in your IDE during development.
    target_compile_definitions(tests PUBLIC RESOURCE_PATH_TEST="${CMAKE_CURRENT_SOURCE_DIR}/res/")
endif()

# Link against map_generator_library (from root CMakeLists.txt) and Catch2.
target_link_libraries(tests PRIVATE map_generator_library Catch2::Catch2WithMain)

include(CTest)
include(Catch)

add_test(NAME tests
        COMMAND $<TARGET_FILE:tests>)
