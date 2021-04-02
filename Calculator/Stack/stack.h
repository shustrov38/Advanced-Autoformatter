#ifndef ADVANCED_CALCULATOR_STACK_H
#define ADVANCED_CALCULATOR_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "../Operations/ops.h"

#define STACK_H_INIT_CAPACITY 4
#define STDATA_VAR_LEN 100

typedef struct {
    char *str;
    OpInfo info;
} stData;

typedef struct {
    size_t elementSize;
    int size, capacity;
    stData *data;
} Stack;


Stack *stCreate();

void stPush(Stack *stack, stData data);

stData stTop(Stack *stack);

void stPop(Stack *stack);

void stResize(Stack *stack, int newCap);

void stPrint(Stack *stack);

#endif //ADVANCED_CALCULATOR_STACK_H

