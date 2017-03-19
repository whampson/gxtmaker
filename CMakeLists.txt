cmake_minimum_required(VERSION 2.8)
project(gxtas)

# Ensure GCC is correctly identified
if(NOT CMAKE_COMPILER_IS_GNUCC AND CMAKE_COMPILER_ID MATCHES "GNU")
    set(CMAKE_COMPILER_IS_GNUCC 1)
endif()

# Ensure Clang is correctly identified
if(NOT CMAKE_COMPILER_IS_CLANGCC AND CMAKE_COMPILER_ID MATCHES "Clang")
    set(CMAKE_COMPILER_IS_CLANGCC 1)
endif()

# Set compiler options
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_CLANGCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11 -Wall")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
endif()

# Create source files list
file(GLOB SOURCE "src/*.c")
file(GLOB HEADERS "src/*.h")

# Target definition
add_executable(${PROJECT_NAME} ${SOURCE} ${HEADERS})
