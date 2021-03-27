#include "RPN.h"

rpnProcessor *rpnProcInit(int elementSize) {
    rpnProcessor *stackVault = malloc(sizeof(rpnProcessor));
    stackVault->numStack = stCreate(elementSize);
    stackVault->opStack = stCreate(elementSize);
    stackVault->finalStack = stCreate(elementSize);
    assert(stackVault != NULL && "bad mem allocate");
    return stackVault;
}


Stack *rpnFunc(rpnProcessor *stack, char **string, int size) {
    int openBracket = 0;

    assert(size > 0 && string != NULL);
    for (int i = 0; i < size; ++i) {
        if (IS_VAR(string[i]) || IS_CONST(string[i]) || getOpID(string[i]) == NUM) {
            stPush(stack->finalStack, string[i]);
        }

        if (!strcmp(string[i], ",") && strcmp(stTop(stack->opStack), "(") != 0) {
            stPush(stack->finalStack, stTop(stack->opStack));
            stPop(stack->opStack);
        }

        if (IS_UOPER(string[i]) || (IS_OPER(string[i]) || IS_FUNC_1ARG(string[i]) || IS_FUNC_2ARG(string[i]))) {
            if (stack->opStack->size != 0 && (IS_UOPER(stTop(stack->opStack)) || IS_OPER(stTop(stack->opStack)) ||
                                              IS_FUNC_1ARG(stTop(stack->opStack)) ||
                                              IS_FUNC_2ARG(stTop(stack->opStack)))) {
                if (IS_PWR(string[i])) {
                    stPush(stack->opStack, string[i]);
                    ++i;
                    if (IS_VAR(string[i]) || IS_CONST(string[i]) || getOpID(string[i]) == NUM) {
                        stPush(stack->finalStack, string[i]);
                    } else if (IS_OPER(string[i]) || IS_FUNC_1ARG(string[i]) ||
                               IS_FUNC_2ARG(string[i])) {
                        stPush(stack->opStack, string[i]);
                    }
                } else {
                    if (PRIORITY(string[i]) > PRIORITY(stTop(stack->opStack))) {
                        stPush(stack->opStack, string[i]);
                    } else {
                        if (IS_FUNC(string[i]) && IS_UOPER(stTop(stack->opStack))){
                            stPush(stack->opStack, string[i]);
                        } else {
                            while (stack->opStack->size != 0 &&
                                   (PRIORITY(string[i]) <= PRIORITY(stTop(stack->opStack)))) {
                                stPush(stack->finalStack, stTop(stack->opStack));
                                stPop(stack->opStack);
                            }
                        }
                        stPush(stack->opStack, string[i]);
                    }
                }
            } else {
                if (IS_PWR(string[i])) {
                    stPush(stack->opStack, string[i]);
                    ++i;
                    if (IS_VAR(string[i]) || IS_CONST(string[i]) || getOpID(string[i]) == NUM) {
                        stPush(stack->finalStack, string[i]);
                    } else if (IS_OPER(string[i]) || IS_FUNC_1ARG(string[i]) ||
                               IS_FUNC_2ARG(string[i]) || !strcmp(string[i], "^")) {
                        stPush(stack->opStack, string[i]);
                    }
                } else stPush(stack->opStack, string[i]);
            }
        }

        if (strcmp(string[i], "(") == 0) {
            openBracket++;
            stPush(stack->opStack, string[i]);
        }

        if (strcmp(string[i], ")") == 0) {
            while (strcmp(stTop(stack->opStack), "(") != 0) {
                stPush(stack->finalStack, stTop(stack->opStack));
                stPop(stack->opStack);
            }
            stPop(stack->opStack);
            if (stack->opStack->size && (IS_UOPER(stTop(stack->opStack)) || IS_FUNC_1ARG(stTop(stack->opStack)) ||
                                         IS_FUNC_2ARG(stTop(stack->opStack)))) {
                stPush(stack->finalStack, stTop(stack->opStack));
                stPop(stack->opStack);
            }
            openBracket--;
        }
#ifdef __RPN_DEBUG__
        printf("op");
        stPrint(stack->opStack);
        printf("final");
        stPrint(stack->finalStack);
        printf("i = %d\n", i);
#endif //__RPN_DEBUG__
    }

    while (stack->opStack->size != 0) {
        stPush(stack->finalStack,
               stTop(stack->opStack));
        stPop(stack->opStack);
    }
#ifdef __RPN_DEBUG__
    stPrint(stack->finalStack);
#endif //__RPN_DEBUG__

    return stack->finalStack;
}