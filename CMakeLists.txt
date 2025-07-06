cmake_minimum_required(VERSION 3.10)
project(NeuroRF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# headers
include_directories(include)
include_directories(CustomNN/headers)

# NeuroRF source files
file(GLOB_RECURSE NEURORF_SRC "src/*.cpp")

# neural network librry
add_subdirectory(CustomNN)

# executable
add_executable(NeuroRF ${NEURORF_SRC})

# neural network library
target_link_libraries(NeuroRF NeuralNetwork)

# weights directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/weights)