cmake_minimum_required(VERSION 3.17)
project(Advanced_Autoformatter C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--stack,10000000")


add_subdirectory(Calculator)
add_subdirectory(Autoformatter)


#add_compile_definitions(__PARSER_OUT_DEBUG__)     # print data collected from parser
#add_compile_definitions(__CYCLE_DETECTOR_DEBUG__) # print all lines after converting
#add_compile_definitions(__INTERPRET_DEBUG__)

file(GLOB SOURCES *.c *.h)
add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES})

include_directories(Calculator Autoformatter)
target_link_libraries(${CMAKE_PROJECT_NAME} CalculatorStaticLibrary)
target_link_libraries(${CMAKE_PROJECT_NAME} AutoformatterStaticLibrary)
