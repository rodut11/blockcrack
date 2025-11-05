cmake_minimum_required(VERSION 4.0)
project(C C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(..)

add_executable(C
        src/main.c
        src/blocks.h
        src/blocks.c
        utils/util_blocks.h
        utils/util_blocks.c
        utils/debug/debug_print_blocks.c
        utils/debug/debug.h
        include/ANSI-Color-Codes.h
        utils/waydroid/C/waydroid_comm.c
        utils/waydroid/C/waydroid_comm.h
        utils/waydroid/vision/get_grid_origin.cpp
        utils/waydroid/vision/get_grid_origin.h
)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(C PRIVATE ${OpenCV_LIBS})
