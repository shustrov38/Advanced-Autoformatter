cmake_minimum_required(VERSION 3.17)
project(Advanced_Autoformatter C)

set(CMAKE_C_STANDARD 11)

# add_compile_definitions(__LINE_MAKER_DEBUG__)

file(GLOB SOURCES *.c *.h)

add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES})