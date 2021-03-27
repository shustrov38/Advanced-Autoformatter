#ifndef ADVANCED_CALCULATOR_STACK_H
#define ADVANCED_CALCULATOR_STACK_H





#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>

#define STACK_H_INIT_CAPACITY 4

typedef struct {
    size_t elementSize;
    int size, capacity;
    char **data;
} Stack;


Stack *stCreate();

Stack *stCreate(int elementSize);


void stPush(Stack *stack, char *data);

char *stTop(Stack *stack);

void stPop(Stack *stack);

void stResize(Stack *stack, int newCap);

void stPrint(Stack *stack);

#endif //ADVANCED_CALCULATOR_STACK_H

