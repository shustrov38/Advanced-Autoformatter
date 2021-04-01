#ifndef ADVANCED_CALCULATOR_TREE_H
#define ADVANCED_CALCULATOR_TREE_H


#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <complex.h>

#include "../Stack/stack.h"
#include "../Operations/ops.h"
#include "../Parser/parser.h"
#include "../Operations/constructions.h"

typedef enum {
    OPERATION,
    FUNCTION1,
    FUNCTION2,
    BASIC
} NodeState;

typedef struct node_t {
    StData value;
    NodeState state;
    double complex result;
    size_t elementSize;
    struct node_t *left, *right;
} Node;

Node *nodeInit();

void opTreeGen(Node *node, Stack *stack);

double complex opTreeCalc(Node *node, Expression *e, int ind, int n);

void opTreePrint(Node *node, Node *parent);


#endif //ADVANCED_CALCULATOR_TREE_H
