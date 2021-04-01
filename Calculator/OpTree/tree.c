#include "tree.h"

Node *nodeInit() {
    Node *node = (Node *) malloc(sizeof(Node));
    assert(node != NULL && "bad mem allocate");
    node->elementSize = sizeof(StData);
    node->left = node->right = NULL;
    return node;
}

void opTreeGen(Node *node, Stack *stack) {
    assert(node != NULL && "bad root");
    assert(stack != NULL && stack->size != 0 && "bad stack");

    // recursive end condition
    if (stack->size == 0) return;

    StData temp = stTop(stack);
    memcpy(&node->value, &temp, sizeof(StData));
    stPop(stack);

    if (IS_OPER(node->value.data_id) || IS_UOPER(node->value.data_id)) {
        node->state = OPERATION;
    } else if (IS_FUNC_1ARG(node->value.data_id)) {
        node->state = FUNCTION1;
    } else if (IS_FUNC_2ARG(node->value.data_id)) {
        node->state = FUNCTION2;
    } else /* var or const */ {
        node->state = BASIC;
    }

    if (node->state != BASIC) {
        node->right = nodeInit();
        opTreeGen(node->right, stack);
        // unary and func1 require one argument
        if (node->state != FUNCTION1 && !IS_UOPER(node->value.data_id)) {
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

void opTreePrint(Node *node, Node *parent) {
    if (node == NULL) return;
    int need;
    switch (node->state) {
        case OPERATION:
            need = (parent != NULL) && ((PRIORITY(node->value.data_id) == SUM && PRIORITY(parent->value.data_id) == PROD) ||
                                        (node->value.data_id != VAR && node->value.data_id != NUM &&
                                         PRIORITY(parent->value.data_id) == POWER));
            if (need) printf("(");
            opTreePrint(node->left, node);
            printf("%s", node->value.data_str);
            opTreePrint(node->right, node);
            if (need) printf(")");
            break;
        case FUNCTION1:
            printf("%s(", node->value.data_str);
            opTreePrint(node->right, node);
            printf(")");
            break;
        case FUNCTION2:
            printf("%s(", node->value.data_str);
            opTreePrint(node->left, node);
            printf(",");
            opTreePrint(node->right, node);
            printf(")");
            break;
        case BASIC:
            printf("%s", node->value.data_str);
            break;
    }
}
