cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(dxdemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

# Directory where exe,dll and static library saved
set(DXDEMO_BINARY_DIR ${CMAKE_BINARY_DIR}/bin-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DXDEMO_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DXDEMO_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DXDEMO_BINARY_DIR})

# Find dependencies
#find_package(glew REQUIRED)

add_executable(dxdemo dxdemo.cpp)
target_link_libraries(dxdemo d3d9 d3dx9 winmm)
