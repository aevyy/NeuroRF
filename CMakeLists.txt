
cmake_minimum_required(VERSION 3.10)
project(NeuroRF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# NeuroRF headers
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/CustomNN/headers)
include_directories(${CMAKE_SOURCE_DIR}/radix-2-DIT)

# NeuroRF source files
file(GLOB_RECURSE NEURORF_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Add subdirectories for libraries
add_subdirectory(CustomNN)
add_subdirectory(radix-2-DIT)

# Main executable
add_executable(NeuroRF ${NEURORF_SRC})

# Link libraries
target_link_libraries(NeuroRF NeuralNetwork Radix2DIT)

# Weights directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/weights)