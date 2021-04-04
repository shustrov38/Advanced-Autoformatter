#include "parser.h"

#define MAX_ARRAY_SIZE 100
#define MAX_E_SIZE 100
#define MAX_STR_SIZE 100

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

void initExpression(Expression *E) {
    for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
        E[i].code = (char **) malloc(MAX_E_SIZE * sizeof(char *));
        for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
            E[i].code[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].code[j], 0, MAX_V_NAME_SIZE);
        }
    }
}

Expression *createExpressions() {
    Expression *tmp = (Expression *) malloc(MAX_ARRAY_SIZE * sizeof(Expression));
    assert((tmp) && "null ptr at creating expressions array");
    initExpression(tmp);
    return tmp;
}

int addExpression(Expression *expr, int exprSize, char **src, int srcSize) {
    // check for ';' at the end of code line
    for (int j = 0; j < srcSize; ++j) {
        if (!strcmp(src[j], ";")) {
            srcSize = j;
            break;
        }
    }

    int addBracket = 0;
    int exprInd = 0;
    int i = 0;
    if (srcSize > 1 && strcmp(src[1], "==") != 0 && strcmp(src[1], "=") != 0 && src[1][strlen(src[1]) - 1] == '=') {
        src[1][strlen(src[1]) - 1] = 0;
        strcpy(expr[exprSize].code[exprInd++], src[0]);
        strcpy(expr[exprSize].code[exprInd++], "=");
        if (strlen(src[1]) > 0) {
            strcpy(expr[exprSize].code[exprInd++], src[0]);
            strcpy(expr[exprSize].code[exprInd++], src[1]);
            strcpy(expr[exprSize].code[exprInd++], "(");
            addBracket = 1;
        }
        i = 2;
    }

    for (; i < srcSize; ++i) {
        strcpy(expr[exprSize].code[exprInd++], src[i]);
    }

    if (addBracket) {
        strcpy(expr[exprSize].code[exprInd++], ")");
    }

    expr[exprSize].size = exprInd;
    return exprInd;
}

void destroyExpressionsArray(Expression *E) {
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        for (int j = 0; j < MAX_E_SIZE; ++j) {
            free(E[i].code[j]);
        }
        free(E[i].code);
    }
}