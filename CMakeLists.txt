# Copyright 2025 Dan Williams. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this
# software and associated documentation files (the "Software"), to deal in the Software
# without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or
# substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
# PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
# OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
cmake_minimum_required(VERSION 3.11)

project(ncurses_prog_bar)

# Flags for C and C++
set(c_cppFlags
    -O3
    -Wall
    -Werror
    -fdiagnostics-color=always)

# Flags for just C++
set(cppOnlyFlags
   -std=c++14)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
   list(APPEND c_cppFlags -Wno-unused-variable)
endif()

# Pre-processor directives
set(defines
   )

# Include paths
set(includes
   .
   )

# Libraries
set(libraries
   ncurses
   )

# Source files
set(source
    main.cpp)

add_executable(ncurses_prog_bar ${source})

# Specify Flags, defines, and includes
target_compile_options(ncurses_prog_bar PRIVATE ${c_cppFlags})
target_compile_options(ncurses_prog_bar PRIVATE $<$<COMPILE_LANGUAGE:CXX>:${cppOnlyFlags}>)
target_compile_definitions(ncurses_prog_bar PRIVATE ${defines})
target_include_directories(ncurses_prog_bar PUBLIC ${includes})
target_link_libraries(ncurses_prog_bar PUBLIC ${libraries})
