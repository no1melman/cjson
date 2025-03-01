cmake_minimum_required(VERSION 3.16)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the compiler to Clang
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use Ninja as the generator
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_GENERATOR "Ninja")

if(WIN32)
  message(STATUS "On Windows:")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target x86_64-w64-mingw32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target x86_64-w64-mingw32")
endif()

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  message(STATUS "ccache: found")

  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
else()
  message(STATUS "no ccache")
endif()

project(cjson C)

# Specify the source files
set(SOURCES src/cjson.c src/utils.c src/utf8JsonReader.c)

# Specify the header files
set(HEADERS include/cjson.h include/utils.h include/types.h
            include/utf8JsonReader.h)

# Create the executable
add_executable(cjson ${SOURCES} ${HEADERS})

# Include directories
include_directories(include)

# Enable warnings
target_compile_options(cjson PRIVATE -Wall -Wextra -Wpedantic)

# Set output directory
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
