#include "parser.h"

#define MAX_ARRAY_SIZE 100
#define MAX_E_SIZE 100
#define MAX_STR_SIZE 100

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

void printPseudoStr(char *pStr) {
    fprintf(stderr, "%s\n", pStr);
}

void initExpression(Expression *E) {
    assert((E) && "null ptr at expressions array while init");
    for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
        E[i].formula = (char **) malloc(MAX_E_SIZE * sizeof(char *));
        E[i].rawFormula = (char *) malloc(MAX_E_SIZE * sizeof(char));
        memset(E[i].rawFormula, 0, MAX_E_SIZE);
        for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
            E[i].formula[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].formula[j], 0, MAX_V_NAME_SIZE);
        }
        E[i].varName = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
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
    if (srcSize > 1 && src[1][strlen(src[1]) - 1] == '=') {
        src[1][strlen(src[1]) - 1] = 0;
        strcpy(expr[exprSize].formula[exprInd++], src[0]);
        strcpy(expr[exprSize].formula[exprInd++], "=");
        if (strlen(src[1]) > 0) {
            strcpy(expr[exprSize].formula[exprInd++], src[0]);
            strcpy(expr[exprSize].formula[exprInd++], src[1]);
            strcpy(expr[exprSize].formula[exprInd++], "(");
            addBracket = 1;
        }
        i = 2;
    }

    // store increments for variables
    char vars[MAX_ARRAY_SIZE][MAX_STR_SIZE];
    int vals[MAX_ARRAY_SIZE] = {0};
    int count = 0;

    for (; i < srcSize; ++i) {
        strcat(expr[exprSize].rawFormula, src[i]);
        strcpy(expr[exprSize].formula[exprInd++], src[i]);
    }

    if (addBracket) {
        strcpy(expr[exprSize].formula[exprInd++], ")");
    }

    return exprInd;
}

void destroyExpressionsArray(Expression *E) {
    assert((E) && "null ptr, lul, nothing to delete");
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        for (int j = 0; j < MAX_E_SIZE; ++j) {
            free(E[i].formula[j]);

        }
        free(E[i].varName);
        free(E[i].formula);
    }
    free(E);
}