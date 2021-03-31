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

    // check for ';' at the end of code line
    for (int i = 0; i < size; ++i) {
        if (!strcmp(string[i], ";")) {
            size = i;
            break;
        }
    }

    // ['x', '=', '5'] -> [VAR, EQL, NUM]
    OpID *id = getLineOfIDs(string, size);

    int openBracket = 0;
    for (int i = 0; i < size; ++i) {
        if (IS_VAR(id[i]) || IS_CONST(id[i]) || id[i] == NUM) {
            StData data = {string[i], id[i]};
            stPush(stack->finalStack, data);
        }

        if (!strcmp(string[i], ",") && strcmp(stTop(stack->opStack).data_str, "(") != 0) {
            stPush(stack->finalStack, stTop(stack->opStack));
            stPop(stack->opStack);
        }

        if (IS_UOPER(id[i]) || (IS_OPER(id[i]) || IS_FUNC_1ARG(id[i]) || IS_FUNC_2ARG(id[i]))) {
            if (stack->opStack->size != 0 && (IS_UOPER(stTop(stack->opStack).data_id) ||
                                              IS_OPER(stTop(stack->opStack).data_id) ||
                                              IS_FUNC_1ARG(stTop(stack->opStack).data_id) ||
                                              IS_FUNC_2ARG(stTop(stack->opStack).data_id))) {
                if (IS_PWR(id[i])) {
                    StData data1 = {string[i], id[i]};
                    stPush(stack->opStack, data1);
                    ++i;
                    StData data2 = {string[i], id[i]};
                    if (IS_VAR(id[i]) || IS_CONST(id[i]) || id[i] == NUM) {
                        stPush(stack->finalStack, data2);
                    } else if (IS_OPER(id[i]) || IS_FUNC_1ARG(id[i]) || IS_FUNC_2ARG(id[i])) {
                        stPush(stack->opStack, data2);
                    }
                } else {
                    if (PRIORITY(id[i]) > PRIORITY(stTop(stack->opStack).data_id)) {
                        StData data1 = {string[i], id[i]};
                        stPush(stack->opStack, data1);
                    } else {
                        if (IS_FUNC(id[i]) && IS_UOPER(stTop(stack->opStack).data_id)) {
                            StData data1 = {string[i], id[i]};
                            stPush(stack->opStack, data1);
                        } else {
                            while (stack->opStack->size != 0 &&
                                   (PRIORITY(id[i]) <= PRIORITY(stTop(stack->opStack).data_id))) {
                                stPush(stack->finalStack, stTop(stack->opStack));
                                stPop(stack->opStack);
                            }
                        }
                        StData data1 = {string[i], id[i]};
                        stPush(stack->opStack, data1);
                    }
                }
            } else {
                if (IS_PWR(id[i])) {
                    StData data1 = {string[i], id[i]};
                    stPush(stack->opStack, data1);
                    ++i;
                    StData data2 = {string[i], id[i]};
                    if (IS_VAR(id[i]) || IS_CONST(id[i]) || id[i] == NUM) {
                        stPush(stack->finalStack, data2);
                    } else if (IS_OPER(id[i]) || IS_FUNC_1ARG(id[i]) ||
                               IS_FUNC_2ARG(id[i]) || !strcmp(string[i], "^")) {
                        stPush(stack->opStack, data2);
                    }
                } else {
                    StData data1 = {string[i], id[i]};
                    stPush(stack->opStack, data1);
                }
            }
        }

        if (strcmp(string[i], "(") == 0) {
            openBracket++;
            StData data = {string[i], id[i]};
            stPush(stack->opStack, data);
        }

        if (strcmp(string[i], ")") == 0) {
            while (strcmp(stTop(stack->opStack).data_str, "(") != 0) {
                stPush(stack->finalStack, stTop(stack->opStack));
                stPop(stack->opStack);
            }
            stPop(stack->opStack);
            if (stack->opStack->size && (IS_UOPER(stTop(stack->opStack).data_id) ||
                                         IS_FUNC_1ARG(stTop(stack->opStack).data_id) ||
                                         IS_FUNC_2ARG(stTop(stack->opStack).data_id))) {
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

    free(id);
    return stack->finalStack;
}