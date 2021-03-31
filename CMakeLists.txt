cmake_minimum_required(VERSION 3.17)
project(Advanced_Autoformatter C)

set(CMAKE_C_STANDARD 11)

# add_compile_definitions(__LINE_MAKER_DEBUG__)     #

file(GLOB SOURCES *.c *.h)

#    add_compile_definitions(__PARSER_DEBUG__) # parser expression debug
#    add_compile_definitions(__GRAPH_DEBUG__ ) # print all graph data
#    add_compile_definitions(__RPN_DEBUG__   ) # debug info for RPN
#    add_compile_definitions(__EXP_DEBUG__   ) # print stack and builded tree

file(GLOB SOURCES_CALCULATOR
        calculator/Stack/stack.?
        calculator/Operations/ops.?
#        calculator/Operations/constructions.?
#        calculator/Parser/parser.?
#        calculator/Graph/graph.?
        calculator/RPN/RPN.?
#        calculator/OpTree/tree.?
        )

add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES} ${SOURCES_CALCULATOR})