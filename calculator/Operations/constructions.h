#ifndef ADVANCED_CALCULATOR_CONSTRUCTIONS_H
#define ADVANCED_CALCULATOR_CONSTRUCTIONS_H

#include <errno.h>

#define _GNU_SOURCE
#include <complex.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "../Parser/parser.h"
#include "ops.h"

double complex toComplex(char *str);

void printNum(double complex value);

double complex fixNegativeZero(double complex a);

double complex idToFunction(char *val, Expression *e, int ind, int n, double complex a, double complex b);

#pragma region OPERATIONS

double complex _umns(double complex a, Expression *e);

double complex _flip(double complex a, Expression *e);

double complex _fact(double complex a, Expression *e);

double complex _sum(double complex a, double complex b, Expression *e);

double complex _sub(double complex a, double complex b, Expression *e);

double complex _mul(double complex a, double complex b, Expression *e);

double complex _div(double complex a, double complex b, Expression *e);

double complex _mod(double complex a, double complex b, Expression *e);

double complex _pwr(double complex a, double complex b, Expression *e);

double complex _and(double complex a, double complex b, Expression *e);

double complex _or(double complex a, double complex b, Expression *e);

double complex _xor(double complex a, double complex b, Expression *e);

#pragma endregion OPERATIONS

#pragma region FUNCTIONS_1ARG

double complex _sin(double complex a, Expression *e);

double complex _cos(double complex a, Expression *e);

double complex _tg(double complex a, Expression *e);

double complex _ctg(double complex a, Expression *e);

double complex _rad(double complex a, Expression *e);

double complex _floor(double complex a, Expression *e);

double complex _ceil(double complex a, Expression *e);

double complex _ln(double complex a, Expression *e);

double complex _log(double complex a, Expression *e);

double complex _sqrt(double complex a, Expression *e);

double complex _abs(double complex a, Expression *e);

double complex _exp(double complex a, Expression *e);

double complex _real(double complex a, Expression *e);

double complex _imag(double complex a, Expression *e);

double complex _mag(double complex a, Expression *e);

double complex _phase(double complex a, Expression *e);

#pragma endregion FUNCTIONS_1ARG

#pragma region FUNCTIONS_2ARG

double complex _pow(double complex a, double complex b, Expression *e);

double complex _min(double complex a, double complex b, Expression *e);

double complex _max(double complex a, double complex b, Expression *e);

double complex _rand(double complex a, double complex b, Expression *e);

#pragma endregion FUNCTIONS_2ARG

#pragma region CONSTANTS

double complex _pi();

double complex _euler();

double complex _j();

#pragma endregion CONSTANTS


#endif //ADVANCED_CALCULATOR_CONSTRUCTIONS_H
