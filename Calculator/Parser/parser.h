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
    char **code;
    int size;
} Expression;

Expression *createExpressions();

int addExpression(Expression *expr, int exprSize, char **src, int srcSize);
void destroyExpressionsArray(Expression *E);

#endif //ADVANCED_CALCULATOR_PARSER_H