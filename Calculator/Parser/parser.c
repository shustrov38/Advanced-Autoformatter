#include "parser.h"

#define MAX_ARRAY_SIZE 100
#define MAX_E_SIZE 100

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

void printPseudoStr(char *pStr) {
    fprintf(stderr, "%s\n", pStr);
}

void initExpression(Expression *E) {
    assert((E) && "null ptr at expressions array while init");
    for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
        E[i].formula = (char **) malloc(MAX_ARRAY_SIZE * sizeof(char *));
        E[i].rawFormula = (char *) malloc(MAX_E_SIZE * sizeof(char));
        memset(E[i].rawFormula, 0, MAX_E_SIZE);
        for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
            E[i].formula[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].formula[j], 0, MAX_V_NAME_SIZE);

        }
        E[i].varName = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));

        for (int j = 0; j < MAX_E_SIZE; ++j) {
            memset(E[i].formula[j], 0, MAX_V_NAME_SIZE);
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
    int exprInd = 0;
    int brF = 0;
    int i = 0;
    if (srcSize > 1 && src[1][strlen(src[1]) - 1] == '=') {
        src[1][strlen(src[1]) - 1] = 0;
        printf("\n%s\n", src[1]);
        strcpy(expr[exprSize].formula[exprInd++], src[0]);
        strcpy(expr[exprSize].formula[exprInd++], "=");
        printf("%d",exprInd);
        if (strlen(src[1])>0){
            strcpy(expr[exprSize].formula[exprInd+1], src[0]);
            strcpy(expr[exprSize].formula[exprInd+2], src[1]);
            strcpy(expr[exprSize].formula[exprInd+3], "(");
            brF = 1;
        }
        i = 2;
    }

    for (; i < srcSize; ++i) {
        strcat(expr[exprSize].rawFormula, src[i]);
        strcpy(expr[exprSize].formula[exprInd++],src[i]);
    }
    //if(brF) strcpy(expr[exprSize].formula[exprInd],")");

    return exprSize + 1;
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