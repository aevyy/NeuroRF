cmake_minimum_required(VERSION 3.10)
project(NeuroRF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)
include_directories(CustomNN/headers)

# source files (excluding problematic ones)
file(GLOB_RECURSE SRC_FILES 
    "src/*.cpp"
    "CustomNN/src/Layer.cpp"
    "CustomNN/src/Matrix.cpp"
    "CustomNN/src/Neuron.cpp"
    "CustomNN/src/NeuralNetwork/*.cpp"
)

# Excluding conflict causing files
list(FILTER SRC_FILES EXCLUDE REGEX "NetworkDemo\\.cpp$|NetworkTester\\.cpp$|NetworkTrainer\\.cpp$")

# executable
add_executable(NeuroRF ${SRC_FILES})

# ensuring the weights directory exists for output files
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/weights)

# Copying configuration files to build directory
# file(GLOB CONFIG_FILES "config/*.json")
# file(COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/config)