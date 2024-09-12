cmake_minimum_required(VERSION "3.27.0")
set(CMAKE_CXX_STANDARD 20)

# TEST PROJECT.
project(map_generator)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(map_generator "${MY_SOURCES}")

set_target_properties(map_generator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

target_link_libraries(map_generator PRIVATE map_generator_library)
