cmake_minimum_required(VERSION 3.17)
project(Advanced_Calculator C)

set(CMAKE_C_STANDARD 11)

set(DEBUG_VAR ON)

if (DEBUG_VAR)
#    add_compile_definitions(__PARSER_DEBUG__) # parser expression debug
#    add_compile_definitions(__GRAPH_DEBUG__ ) # print all graph data
#    add_compile_definitions(__RPN_DEBUG__   ) # debug info for RPN
#    add_compile_definitions(__EXP_DEBUG__   ) # print stack and builded tree
endif ()

file(GLOB SOURCES
        Stack/stack.?
        Operations/ops.?
        Operations/constructions.?
        Parser/parser.?
        Graph/graph.?
        RPN/RPN.?
        OpTree/tree.?)

add_executable(${CMAKE_PROJECT_NAME} main.c ${SOURCES})