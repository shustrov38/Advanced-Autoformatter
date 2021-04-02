#include "stack.h"

Stack *stCreate() {
    Stack *stack = (Stack *) malloc(sizeof(Stack));
    assert(stack != NULL && "bad mem allocate");
    stack->capacity = STACK_H_INIT_CAPACITY;
    stack->elementSize = sizeof(stData);
    stack->size = 0;
    stack->data = (stData *) malloc(STACK_H_INIT_CAPACITY * stack->elementSize);
    assert(stack->data != NULL && "bad mem allocate");
    return stack;
}

void stPush(Stack *stack, stData data) {
    if (stack->size >= stack->capacity) {
        stResize(stack, stack->capacity * 2);
    }
    memcpy(&stack->data[stack->size], &data, stack->elementSize);
    stack->size++;
}

stData stTop(Stack *stack) {
    if (stack->size == 0) {
        fprintf(stderr, "Stack is empty.\n");
        exit(-1);
    }
    return stack->data[stack->size - 1];
}

void stPop(Stack *stack) {
    if (stack->size == 0) {
        fprintf(stderr, "Stack is empty.\n");
        exit(-1);
    }
    stack->size--;
}

void stResize(Stack *stack, int newCap) {
    stack->capacity = newCap;
    stack->data = realloc(stack->data, newCap * stack->elementSize);
}

void stPrint(Stack *stack) {
    printf("stack: [");
    if (stack->size) {
        for (int i = 0; i < stack->size; ++i) {
//            printf("{%s %d}", stack->data[i].data_str, stack->data[i].data_id);
            printf("%s", stack->data[i].str);
            if (i + 1 != stack->size) {
                printf(", ");
            }
        }
    } else {
        printf("EMPTY STACK");
    }
    printf("]\n");
}
