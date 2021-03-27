#ifndef ADVANCED_CALCULATOR_RPN_H
#define ADVANCED_CALCULATOR_RPN_H
#include "../Stack/stack.h"
#include "../Operations/ops.h"
#endif //ADVANCED_CALCULATOR_RPN_H

typedef struct {
    Stack *numStack;
    Stack *opStack;
    Stack *finalStack;
} rpnProcessor;

rpnProcessor *rpnProcInit(int elementSize);

int cmpOpen(char *string1);

int cmpClose(char *string1);

Stack* rpnFunc(rpnProcessor *stack, char** string, int size);