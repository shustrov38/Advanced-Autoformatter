cmake_minimum_required(VERSION 3.17)
project(Advanced_Autoformatter C)

set(CMAKE_C_STANDARD 11)

add_executable(Advanced_Autoformatter main.c LineMakerAlg/lineMaker.c LineMakerAlg/lineMaker.h optionFuncs/optionFunctions.c optionFuncs/optionFunctions.h)