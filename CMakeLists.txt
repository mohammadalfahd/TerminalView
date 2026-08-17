cmake_minimum_required(VERSION 3.16)

project(TerminalView VERSION 1.0 LANGUAGES CXX)

# =========================
# Options
# =========================
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

# =========================
# C++ standard
# =========================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# =========================
# Dependencies
# =========================
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

# =========================
# Sources
# =========================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(TerminalView ${SOURCES})

target_include_directories(TerminalView PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(TerminalView PRIVATE
    ixwebsocket
    ZLIB::ZLIB
    OpenSSL::SSL
    OpenSSL::Crypto
    CURL::libcurl
    Threads::Threads
)

# =========================
# Warnings
# =========================
target_compile_options(TerminalView PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# =========================
# Sanitizers
# =========================
if(ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ASan and TSan cannot be enabled together.")
endif()

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    target_compile_options(TerminalView PRIVATE
        -fsanitize=address
        -fno-omit-frame-pointer
        -g
        -O1
    )
    target_link_options(TerminalView PRIVATE
        -fsanitize=address
    )
endif()

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")
    target_compile_options(TerminalView PRIVATE
        -fsanitize=thread
        -fno-omit-frame-pointer
        -g
        -O1
    )
    target_link_options(TerminalView PRIVATE
        -fsanitize=thread
    )
endif()