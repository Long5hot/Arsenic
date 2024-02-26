cmake_minimum_required(VERSION 3.10)
project(Arsenic)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Directories
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Source files
file(GLOB_RECURSE SRCS ${SRC_DIR}/*.cpp)

# Include directories
include_directories(${INCLUDE_DIR})

# Add executable
add_executable(arsenic ${SRCS})

# Set output directory for executable
set_target_properties(arsenic PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin
)

# Set compiler flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
else()
    message("Release mode")
    target_compile_options(arsenic PRIVATE -O2)
endif()

