cmake_minimum_required(VERSION 3.16)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the compiler to Clang
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_COMPILER gcc)
# Use Ninja as the generator
set(CMAKE_GENERATOR "Ninja")

find_program(CCACHE_PROGRAM ccache)

if (CCACHE_PROGRAM)
    message(STATUS "ccache: found")

    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
else ()
    message(STATUS "no ccache")
endif ()

project(cjson C)

# Specify the source files
set(SOURCES src/cjson.c src/utils.c src/utf8JsonReader.c src/hasher.c src/hashset.c src/stack.c)

# Specify the header files
set(HEADERS include/cjson.h include/utils.h include/types.h
        include/stack.h include/utf8JsonReader.h include/hasher.h include/hashset.h include/stack.h)
# Create the executable
add_executable(cjson ${SOURCES} ${HEADERS})

# Include directories
include_directories(include)

# Enable warnings
target_compile_options(cjson PRIVATE -Wall -Wextra -Wpedantic)

# Set output directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
