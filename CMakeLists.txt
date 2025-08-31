cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 99)

project(TEdit C)

add_executable(TEdit main.c)

target_include_directories(TEdit PRIVATE ${CMAKE_SOURCE_DIR}/include/)


