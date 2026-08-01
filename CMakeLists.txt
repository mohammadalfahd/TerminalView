cmake_minimum_required(VERSION 3.16)

project(TerminalView VERSION 1.0 LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings
add_compile_options(-Wall -Wextra -O2)

# Find packages
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

# Collect all source files
file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Create executable
add_executable(TerminalView ${SOURCES})

# Include directory
target_include_directories(TerminalView
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(TerminalView
    PRIVATE
        CURL::libcurl
        Threads::Threads
)