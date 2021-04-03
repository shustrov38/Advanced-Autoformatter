#include "RPN.h"

rpnProcessor *rpnProcInit() {
    rpnProcessor *stackVault = malloc(sizeof(rpnProcessor));
    stackVault->numStack = stCreate();
    stackVault->opStack = stCreate();
    stackVault->finalStack = stCreate();
    assert(stackVault != NULL && "bad mem allocate");
    return stackVault;
}


Stack *rpnFunc(rpnProcessor *stack, char **string, int size) {
    assert(size > 0 && string != NULL);

    // ['x', '=', '5'] -> [{VAR NULL}, {EQL NULL}, {NUM NULL}]
    OpInfo *info = getLineOfIDs(string, size);
    for (int i = 0; i < size; ++i){
        printf("[%s , %s]\n", string[i], (strlen(info[i].relatedTo) == 0 ? "NULL" : info[i].relatedTo));
    }

    int openBracket = 0;
    for (int i = 0; i < size; ++i) {
        if (IS_VAR(info[i].id) || IS_CONST(info[i].id) || info[i].id == NUM) {
            stData data = {string[i],info[i]};
            stPush(stack->finalStack, data);
        }

        if (!strcmp(string[i], ",") && strcmp(stTop(stack->opStack).str, "(") != 0) {
            stPush(stack->finalStack, stTop(stack->opStack));
            stPop(stack->opStack);
        }

        if (IS_UOPER(info[i].id) || (IS_OPER(info[i].id) || IS_FUNC_1ARG(info[i].id) || IS_FUNC_2ARG(info[i].id))) {
            if (stack->opStack->size != 0 && (IS_UOPER(stTop(stack->opStack).info.id) ||
                                              IS_OPER(stTop(stack->opStack).info.id) ||
                                              IS_FUNC_1ARG(stTop(stack->opStack).info.id) ||
                                              IS_FUNC_2ARG(stTop(stack->opStack).info.id))) {
                stData data1 = {string[i],info[i]};
                if (IS_PWR(info[i].id)) {
                    stPush(stack->opStack, data1);
                    ++i;
                    stData data2 = {string[i],info[i]};
                    if (IS_VAR(info[i].id) || IS_CONST(info[i].id) || info[i].id == NUM) {
                        stPush(stack->finalStack, data2);
                    } else if (IS_OPER(info[i].id) || IS_FUNC_1ARG(info[i].id) || IS_FUNC_2ARG(info[i].id)) {
                        stPush(stack->opStack, data2);
                    }
                } else {
                    if (PRIORITY(info[i].id) > PRIORITY(stTop(stack->opStack).info.id)) {
                        stPush(stack->opStack, data1);
                    } else {
                        if (IS_FUNC(info[i].id) && IS_UOPER(stTop(stack->opStack).info.id)) {
                            stPush(stack->opStack, data1);
                        } else {
                            while (stack->opStack->size != 0 &&
                                   (PRIORITY(info[i].id) <= PRIORITY(stTop(stack->opStack).info.id))) {
                                stPush(stack->finalStack, stTop(stack->opStack));
                                stPop(stack->opStack);
                            }
                        }
                        stPush(stack->opStack, data1);
                    }
                }
            } else {
                stData data1 = {string[i],info[i]};
                if (IS_PWR(info[i].id)) {
                    stPush(stack->opStack, data1);
                    ++i;
                    stData data2 = {string[i],info[i]};
                    if (IS_VAR(info[i].id) || IS_CONST(info[i].id) || info[i].id == NUM) {
                        stPush(stack->finalStack, data2);
                    } else if (IS_OPER(info[i].id) || IS_FUNC_1ARG(info[i].id) ||
                               IS_FUNC_2ARG(info[i].id) || !strcmp(string[i], "^")) {
                        stPush(stack->opStack, data2);
                    }
                } else {
                    stPush(stack->opStack, data1);
                }
            }
        }

        if (strcmp(string[i], "(") == 0) {
            openBracket++;
            stData data = {string[i],info[i]};
            stPush(stack->opStack, data);
        }

        if (strcmp(string[i], ")") == 0) {
            while (strcmp(stTop(stack->opStack).str, "(") != 0) {
                stPush(stack->finalStack, stTop(stack->opStack));
                stPop(stack->opStack);
            }
            stPop(stack->opStack);
            if (stack->opStack->size && (IS_UOPER(stTop(stack->opStack).info.id) ||
                                         IS_FUNC_1ARG(stTop(stack->opStack).info.id) ||
                                         IS_FUNC_2ARG(stTop(stack->opStack).info.id))) {
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

    free(info);
    return stack->finalStack;
}