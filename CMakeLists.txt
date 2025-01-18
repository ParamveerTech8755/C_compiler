# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(C_compiler VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

# Add the executable
add_executable(mycompiler src/token.c src/lexer.c src/parser.c src/utils.c src/mycompiler.c src/include/customstring.c src/include/errors.c src/include/components/program.c src/include/components/function.c src/include/components/statement.c src/include/components/expression.c)

# Include additional directories if needed
include_directories(src)

#add_custom_command(TARGET mycompiler POST_BUILD
#    COMMAND mycompiler ../examples/example1.c
#   COMMENT "Running the executable after build"
#)
