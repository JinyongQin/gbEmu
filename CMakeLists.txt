project(gbemu LANGUAGES C)
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

add_definitions("-Wall -g")
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
# find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# find SDL2_ttf
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})


#sub dir
add_subdirectory(gbemu) # main.c
add_subdirectory(test)
add_subdirectory(lib) # build emu as lib

# test unit
enable_testing()
add_test(NAME test_gbemu COMMAND test_gbemu)