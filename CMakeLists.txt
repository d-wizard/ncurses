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
