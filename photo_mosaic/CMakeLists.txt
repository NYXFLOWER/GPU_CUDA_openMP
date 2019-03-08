cmake_minimum_required(VERSION 3.13)
project(photo_mosaic)

set(CMAKE_CXX_STANDARD 17)

add_executable(photo_mosaic mosaic.c my_main.c mosaic.h)