cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(dxdemo)

# Include
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Application
add_subdirectory(app)
