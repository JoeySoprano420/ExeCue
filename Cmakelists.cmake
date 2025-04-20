cmake_minimum_required(VERSION 3.18)
project(ExecueCompiler LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUE_VERSION 1.0)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler options
add_compile_options(-Wall -Wextra -Wpedantic)

# Source files
file(GLOB_RECURSE SOURCES
     src/*.cpp
     src/*.h
)

# Executable
add_executable(execue ${SOURCES})

# Include directories
target_include_directories(execue PRIVATE src/)

# Definitions
add_definitions(-DEXECUE_VERSION=\"${EXECUE_VERSION}\")
