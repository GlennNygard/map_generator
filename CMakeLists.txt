cmake_minimum_required(VERSION "3.27.0")
set(CMAKE_CXX_STANDARD 20)
option(PRODUCTION_BUILD "Make this a production build." OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(map_generator)

# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" "${MY_SOURCES}")

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage_definitions/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/map/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/wfc/")
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/")

if(PRODUCTION_BUILD)
    # Setup the asset path macro to be in the root folder of the executable.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCE_PATH="./resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC OUTPUT_PATH="./output/")

    # Remove the option to debug asserts.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
    # This is useful to get an ASSETS_PATH in your IDE during development.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC OUTPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/output/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)


# External tools.

# ExternalProject will download and configure dependencies on build,
# after cmake configures the project.
include(ExternalProject)
# FetchContent makes dependency available before project configuration.
include(FetchContent)

#set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external)

ExternalProject_Add(cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/cxxopts
)

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${EXTERNAL_INSTALL_LOCATION}/cxxopts/include/")

# ExternalProject_Add(yaml-cpp
#     GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}/yaml-cpp
# )
# target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${EXTERNAL_INSTALL_LOCATION}/yaml-cpp/include/")

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master).
    GIT_TAG master
)
FetchContent_MakeAvailable(yaml-cpp)

# The library or executable that require yaml-cpp library.
target_link_libraries("${CMAKE_PROJECT_NAME}" PUBLIC yaml-cpp::yaml-cpp)

# Write configuration file back to the source tree.
# configure_file(${CMAKE_SOURCE_DIR}/templates/app_version.hpp.in ${CMAKE_SOURCE_DIR}/include/app_version.hpp)

endif()
