cmake_minimum_required(VERSION "3.27.0")
set(CMAKE_CXX_STANDARD 20)
option(PRODUCTION_BUILD "Make this a production build." OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(MapGenerator)

# MY_SOURCES is defined to be a list of all the source files for my game 
# DON'T ADD THE SOURCES BY HAND, they are already added with this macro
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}" "${MY_SOURCES}")

target_include_directories(MapGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_include_directories(MapGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage/")
target_include_directories(MapGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/foliage_definitions/")
target_include_directories(MapGenerator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/")

if(PRODUCTION_BUILD)
	# Setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/")
    # Setup the ASSETS_PATH macro to be in the root folder of your exe
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC OUTPUT_PATH="./output/")

	# Remove the option to debug asserts.
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	# This is useful to get an ASSETS_PATH in your IDE during development
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC OUTPUT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/output/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 

endif()
