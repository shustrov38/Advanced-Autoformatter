cmake_minimum_required(VERSION 3.17)
project(Advanced_Autoformatter C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(Calculator)
add_subdirectory(Autoformatter)

#add_compile_definitions(__LINE_MAKER_DEBUG__)     # print all lines after converting

file(GLOB SOURCES *.c *.h)
add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES})

include_directories(Calculator Autoformatter)
target_link_libraries(${CMAKE_PROJECT_NAME} CalculatorStaticLibrary)
target_link_libraries(${CMAKE_PROJECT_NAME} AutoformatterStaticLibrary)