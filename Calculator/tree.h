#ifndef ADVANCED_CALCULATOR_TREE_H
#define ADVANCED_CALCULATOR_TREE_H


#include "../libraries.h"
#include "stack.h"
#include "ops.h"
#include "parser.h"
#include "../memory.h"

typedef enum {
    OPERATION,
    FUNCTION1,
    FUNCTION2,
    BASIC
} NodeState;

typedef struct node_t {
    double result;
    stData value;
    NodeState state;
    size_t elementSize;
    struct node_t *left, *right;
} Node;

Node *nodeInit();

void opTreeGen(Node *node, Stack *stack);

double opTreeCalc(Memory *m,Node *root);

void opTreePrint(Node *node, Node *parent);


#endif //ADVANCED_CALCULATOR_TREE_H
