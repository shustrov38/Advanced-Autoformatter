#ifndef ADVANCED_CALCULATOR_TREE_H
#define ADVANCED_CALCULATOR_TREE_H


#include "../libraries.h"
#include "stack.h"
#include "ops.h"
#include "parser.h"

typedef enum {
    OPERATION,
    FUNCTION1,
    FUNCTION2,
    BASIC
} NodeState;

typedef struct node_t {
    stData value;
    NodeState state;
    double complex result;
    size_t elementSize;
    struct node_t *left, *right;
} Node;

Node *nodeInit();

void opTreeGen(Node *node, Stack *stack);

//double complex opTreeCalc(Node *node, Expression *e, int ind, int n);

void opTreePrint(Node *node, Node *parent);


#endif //ADVANCED_CALCULATOR_TREE_H
