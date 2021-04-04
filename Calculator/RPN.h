#ifndef ADVANCED_CALCULATOR_RPN_H
#define ADVANCED_CALCULATOR_RPN_H

#include "../libraries.h"
#include "stack.h"
#include "ops.h"

typedef struct {
    Stack *numStack;
    Stack *opStack;
    Stack *finalStack;
} rpnProcessor;

rpnProcessor *rpnProcInit();

int cmpOpen(char *string1);

int cmpClose(char *string1);

Stack* rpnFunc(rpnProcessor *stack, char** string, int size);

#endif //ADVANCED_CALCULATOR_RPN_H