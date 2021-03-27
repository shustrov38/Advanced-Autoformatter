#include "tree.h"

Node *nodeInit(int elementSize) {
    Node *node = (Node *) malloc(sizeof(Node));
    assert(node != NULL && "bad mem allocate");
    node->elementSize = elementSize;
    node->value = (char *) malloc(node->elementSize);
    assert(node->value != NULL && "bad mem allocate");
    node->left = node->right = NULL;
    return node;
}

void opTreeGen(Node *node, Stack *stack) {
    assert(node != NULL && "bad root");
    assert(stack != NULL && stack->size != 0 && "bad stack");

    // recursive end condition
    if (stack->size == 0) return;

    strcpy(node->value, stTop(stack));
    stPop(stack);

    if (IS_OPER(node->value) || IS_UOPER(node->value)) {
        node->state = OPERATION;
    } else if (IS_FUNC_1ARG(node->value)) {
        node->state = FUNCTION1;
    } else if (IS_FUNC_2ARG(node->value)) {
        node->state = FUNCTION2;
    } else /* var or const */ {
        node->state = BASIC;
    }

    if (node->state != BASIC) {
        node->right = nodeInit(node->elementSize);
        opTreeGen(node->right, stack);
        // unary and func1 require one argument
        if (node->state != FUNCTION1 && !IS_UOPER(node->value)) {
            node->left = nodeInit(node->elementSize);
            opTreeGen(node->left, stack);
        }
    }
}

double complex opTreeCalc(Node *node, Expression *e, int ind, int n) {
    if (node == NULL) return 0;

    double complex a = fixNegativeZero(opTreeCalc(node->left, e, ind, n));
    double complex b = fixNegativeZero(opTreeCalc(node->right, e, ind, n));

    return idToFunction(node->value, e, ind, n, a, b);
}

void opTreePrint(Node *node, Node *parent) {
    if (node == NULL) return;
    int need;
    switch (node->state) {
        case OPERATION:
            need = (parent != NULL) && ((PRIORITY(node->value) == SUM && PRIORITY(parent->value) == PROD) ||
                                        (getOpID(node->value) != VAR && getOpID(node->value) != NUM &&
                                         PRIORITY(parent->value) == POWER));
            if (need) printf("(");
            opTreePrint(node->left, node);
            printf("%s", node->value);
            opTreePrint(node->right, node);
            if (need) printf(")");
            break;
        case FUNCTION1:
            printf("%s(", node->value);
            opTreePrint(node->right, node);
            printf(")");
            break;
        case FUNCTION2:
            printf("%s(", node->value);
            opTreePrint(node->left, node);
            printf(",");
            opTreePrint(node->right, node);
            printf(")");
            break;
        case BASIC:
            printf("%s", node->value);
            break;
    }
}
