#include "tree.h"

Node *nodeInit() {
    Node *node = (Node *) malloc(sizeof(Node));
    assert(node != NULL && "bad mem allocate");
    node->elementSize = sizeof(stData);
    node->left = node->right = NULL;
    return node;
}

void opTreeGen(Node *node, Stack *stack) {
    assert(node != NULL && "bad root");
    assert(stack != NULL && stack->size != 0 && "bad stack");

    // recursive end condition
    if (stack->size == 0) return;

    stData temp = stTop(stack);
    memcpy(&node->value, &temp, sizeof(stData));
    stPop(stack);

    if (IS_OPER(node->value.info.id) || IS_UOPER(node->value.info.id)) {
        node->state = OPERATION;
    } else if (IS_FUNC_1ARG(node->value.info.id)) {
        node->state = FUNCTION1;
    } else if (IS_FUNC_2ARG(node->value.info.id)) {
        node->state = FUNCTION2;
    } else /* var or const */ {
        node->state = BASIC;
    }

    if (node->state != BASIC) {
        node->right = nodeInit();
        opTreeGen(node->right, stack);
        // unary and func1 require one argument
        if (node->state != FUNCTION1 && !IS_UOPER(node->value.info.id)) {
            node->left = nodeInit();
            opTreeGen(node->left, stack);
        }
    }
}

double complex opTreeCalc(Node *node, Expression *e, int ind, int n) {
    if (node == NULL) return 0;

    double complex a = fixNegativeZero(opTreeCalc(node->left, e, ind, n));
    double complex b = fixNegativeZero(opTreeCalc(node->right, e, ind, n));

    return idToFunction(&node->value, e, ind, n, a, b);
}
