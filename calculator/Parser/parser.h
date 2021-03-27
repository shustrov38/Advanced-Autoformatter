#ifndef ADVANCED_CALCULATOR_PARSER_H
#define ADVANCED_CALCULATOR_PARSER_H


#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <complex.h>

#include "../Operations/ops.h"

#define MAX_V_NAME_SIZE 20

typedef struct {
    char *varName; // name of var, null if expression is just a number
    char **formula; // expression splitted to pseudo string
    char *rawFormula;
    char **dependencies; //vars, on which expression depends, according to plain scanning
    int evenDependenciesCnt; // num of dependencies after scan
    int trueDependenciesCnt; // num of dependencies after reverse+BFS
    double complex value;
    int segCnt;

} Expression;

Expression *createExpressions();

int parserReadExpressions(char *filename, Expression *parsedExpr);
void destroyExpressionsArray(Expression *E);

#endif //ADVANCED_CALCULATOR_PARSER_H