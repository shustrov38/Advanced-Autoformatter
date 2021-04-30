cmake_minimum_required(VERSION 3.17)
project(AutoformatterStaticLibrary C)

set(CMAKE_C_STANDARD 11)

#add_compile_definitions(__LINE_MAKER_DEBUG__)     # print all lines after converting

set(HEADER_FILES_AUTOFORMATTER outputFormat.h lineMaker.h optionFunctions.h parser.h)
set(SOURCE_FILES_AUTOFORMATTER outputFormat.c lineMaker.c optionFunctions.c parser.c)

add_library(AutoformatterStaticLibrary STATIC ${HEADER_FILES_AUTOFORMATTER} ${SOURCE_FILES_AUTOFORMATTER})