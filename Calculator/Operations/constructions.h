#ifndef ADVANCED_CALCULATOR_CONSTRUCTIONS_H
#define ADVANCED_CALCULATOR_CONSTRUCTIONS_H

#include <errno.h>

#define _GNU_SOURCE
#include <complex.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../Stack/stack.h"
#include "../Parser/parser.h"
#include "ops.h"

double complex toComplex(char *str);

void printNum(double complex value);

double complex fixNegativeZero(double complex a);

double complex idToFunction(StData *data, Expression *e, int ind, int n, double complex a, double complex b);

#pragma region OPERATIONS

double complex _post_dec(double complex a, Expression *e);

double complex _post_inc(double complex a, Expression *e);

double complex _pref_dec(double complex a, Expression *e);

double complex _pref_inc(double complex a, Expression *e);

double complex _umns(double complex a, Expression *e);

double complex _upls(double complex a, Expression *e);

double complex _flip(double complex a, Expression *e);

double complex _lnot(double complex a, Expression *e);

double complex _sum(double complex a, double complex b, Expression *e);

double complex _sub(double complex a, double complex b, Expression *e);

double complex _mul(double complex a, double complex b, Expression *e);

double complex _div(double complex a, double complex b, Expression *e);

double complex _mod(double complex a, double complex b, Expression *e);

double complex _band(double complex a, double complex b, Expression *e);

double complex _bor(double complex a, double complex b, Expression *e);

double complex _bxor(double complex a, double complex b, Expression *e);

double complex _land(double complex a, double complex b, Expression *e);

double complex _lor(double complex a, double complex b, Expression *e);

double complex _shl(double complex a, double complex b, Expression *e);

double complex _shr(double complex a, double complex b, Expression *e);

double complex _cmpl(double complex a, double complex b, Expression *e);

double complex _cmple(double complex a, double complex b, Expression *e);

double complex _cmpg(double complex a, double complex b, Expression *e);

double complex _cmpge(double complex a, double complex b, Expression *e);

double complex _eqls(double complex a, double complex b, Expression *e);

#pragma endregion OPERATIONS

#pragma region FUNCTIONS_1ARG

double complex _sin(double complex a, Expression *e);

double complex _cos(double complex a, Expression *e);

double complex _tg(double complex a, Expression *e);

double complex _ctg(double complex a, Expression *e);

double complex _floor(double complex a, Expression *e);

double complex _ceil(double complex a, Expression *e);

double complex _log(double complex a, Expression *e);

double complex _sqrt(double complex a, Expression *e);

double complex _abs(double complex a, Expression *e);

double complex _exp(double complex a, Expression *e);

#pragma endregion FUNCTIONS_1ARG

#pragma region FUNCTIONS_2ARG

double complex _pow(double complex a, double complex b, Expression *e);

#pragma endregion FUNCTIONS_2ARG


#endif //ADVANCED_CALCULATOR_CONSTRUCTIONS_H
