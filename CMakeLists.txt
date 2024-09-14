cmake_minimum_required(VERSION "3.27.0")
set(CMAKE_CXX_STANDARD 20)
option(PRODUCTION_BUILD "Make this a production build." OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(map_generator_library)

# Enable testing for this project
enable_testing()

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM MY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Define the target as a shared library
add_library(map_generator_library SHARED "${MY_SOURCES}")

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage_definitions/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/map/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/wfc/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/utility/")

target_include_directories(map_generator_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/")

if(PRODUCTION_BUILD)
    # Setup the asset path macro to be in the root folder of the executable.
    target_compile_definitions(map_generator_library PUBLIC RESOURCE_PATH="./resources/")
    target_compile_definitions(map_generator_library PUBLIC OUTPUT_PATH="./output/")

    # Remove the option to debug asserts.
    target_compile_definitions(map_generator_library PUBLIC PRODUCTION_BUILD=1) 
else()
    # This is useful to get an ASSETS_PATH in your IDE during development.
    target_compile_definitions(map_generator_library PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions(map_generator_library PUBLIC OUTPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/output/")
    target_compile_definitions(map_generator_library PUBLIC PRODUCTION_BUILD=0)
endif()

add_subdirectory(src)


# Testing.

add_subdirectory(tests)


# External tools.


# ExternalProject will download and configure dependencies on build,
# after cmake configures the project.
include(ExternalProject)
# FetchContent makes dependency available before project configuration.
include(FetchContent)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

ExternalProject_Add(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/cxxopts
)

add_dependencies("${CMAKE_PROJECT_NAME}" cxxopts)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${EXTERNAL_INSTALL_LOCATION}/cxxopts/include")

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master).
    GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

# The library or executable that require yaml-cpp library.
target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC yaml-cpp::yaml-cpp)

