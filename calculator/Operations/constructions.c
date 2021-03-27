#include "constructions.h"

#define eps 1e-5

// check real complex part of X to integer
#define IS_INT(X) (fabs(creal(X) - (int)(X)) <= eps)
// check real complex part of X to Y
#define EQR(X, Y) (fabs(creal(X) - (Y)) <= eps)
// check imag complex part of X to Y
#define EQI(X, Y) (fabs(cimag(X) - (Y)) <= eps)
// check for equal two complex numbers
#define EQC(X, Y) (EQR(X, Y) && EQI(X, Y))
// print error
#define ERROR(...) fprintf(stderr, __VA_ARGS__)
// overflow check

#define INF INT_MAX

#define COMPLEX 0
#define DOUBLE 1
#define COMPLEX_AND_DOUBLE 2

double complex toComplex(char *str) {
    int e = (int) strlen(str);
    return (e == 1 && str[0] == 'j' ? 1 : atof(str)) * (str[e - 1] == 'j' ? I : 1);
}

void printImag(double complex imaginary) {
    if (EQI(imaginary, 1)) {
        printf("i");
    } else if (EQI(imaginary, -1)) {
        printf("-i");
    } else {
        if (isinf(cimag(imaginary))) {
            printf("inf");
        } else if (IS_INT(cimag(imaginary))) {
            printf("%di", (int) cimag(imaginary));
        } else {
            printf("%fi", cimag(imaginary));
        }
    }
}

void printNum(double complex value) {
    if (EQI(value, 0)) {
        if (isinf(creal(value))) {
            printf("inf");
        } else if (IS_INT(value)) {
            printf("%d", (int) value);
        } else {
            printf("%f", creal(value));
        }
    } else {
        if (EQR(value, 0)) {
            printImag(cimag(value) * I);
        } else {
            if (isinf(creal(value))) {
                printf("inf");
            } else if (IS_INT(value)) {
                printf("%d", (int) creal(value));
            } else {
                printf("%f", creal(value));
            }
            if (cimag(value) < 0) {
                printf(" - ");
            } else {
                printf(" + ");
            }
            printImag(fabs(cimag(value)) * I);
        }
    }
}

double complex fixNegativeZero(double complex a) {
    if (EQI(a, 0)) a = creal(a) + 0 * I;
    if (EQC(a, 0)) a = 0 + cimag(a);
    return a;
}

double complex idToFunction(char *val, Expression *e, int ind, int n, double complex a, double complex b) {
    switch (getOpID(val)) {
        case UMNS:
            return _umns(b, &e[ind]);
        case FLIP:
            return _flip(b, &e[ind]);
        case FACT:
            return _fact(b, &e[ind]);
        case PLS:
            return _sum(a, b, &e[ind]);
        case MNS:
            return _sub(a, b, &e[ind]);
        case MUL:
            return _mul(a, b, &e[ind]);
        case DIV:
            return _div(a, b, &e[ind]);
        case MOD:
            return _mod(a, b, &e[ind]);
        case PWR:
            return _pwr(a, b, &e[ind]);
        case AND:
            return _and(a, b, &e[ind]);
        case OR:
            return _or(a, b, &e[ind]);
        case XOR:
            return _xor(a, b, &e[ind]);
        case SIN:
            return _sin(b, &e[ind]);
        case COS:
            return _cos(b, &e[ind]);
        case TG:
            return _tg(b, &e[ind]);
        case CTG:
            return _ctg(b, &e[ind]);
        case RAD:
            return _rad(b, &e[ind]);
        case FLR:
            return _floor(b, &e[ind]);
        case CEIL:
            return _ceil(b, &e[ind]);
        case LN:
            return _ln(b, &e[ind]);
        case LOG:
            return _log(b, &e[ind]);
        case SQRT:
            return _sqrt(b, &e[ind]);
        case ABS:
            return _abs(b, &e[ind]);
        case EXP:
            return _exp(b, &e[ind]);
        case REAL:
            return _real(b, &e[ind]);
        case IMAG:
            return _imag(b, &e[ind]);
        case MAG:
            return _mag(b, &e[ind]);
        case PHASE:
            return _phase(b, &e[ind]);
        case POW:
            return _pow(a, b, &e[ind]);
        case MAX:
            return _max(a, b, &e[ind]);
        case MIN:
            return _min(a, b, &e[ind]);
        case RND:
            return _rand(a, b, &e[ind]);
        case PI:
            return _pi();
        case EULER:
            return _euler();
        case J:
            return _j();
        case VAR:
            for (int i = 0; i < n; ++i) {
                if (!strlen(e[i].varName)) continue;
                if (!strcmp(val, e[i].varName)) {
                    return e[i].value;
                }
            }
        default:
            return toComplex(val);
    }
}

void throwException(char *err, Expression *e) {
    ERROR("%s\n", e->rawFormula);
    ERROR("%s", err);
    exit(-1);
}

// type 0 - complex, 1 - double, 2 - complex and double;
void numberException(double complex a, double complex b, Expression *e, char *symbol, int type, int isFunc, int args) {
    int left, right, st, en;
    if (type == 2) {
        st = 0;
        en = 1;
    } else {
        st = en = type;
    }
    for (int isComplex = st; isComplex <= en; ++isComplex) {
        if (isComplex == 0) {
            left = !EQI(a, 0);
            right = !EQI(b, 0);
        } else if (isComplex == 1) {
            left = !IS_INT(a);
            right = !IS_INT(b);
        }
        if (left || right) {
            ERROR("%s\n", e->rawFormula);
            ERROR("%s '%s' is not define for %s %s%s",
                  (isFunc ? "Function" : "Operation"),
                  symbol,
                  (isComplex == 0 ? "complex" : "floating point"),
                  (isFunc ? "argument" : "operand"),
                  (args == 2 ? "s. " : ". "));
            if (args == 2) {
                if (left && right) {
                    ERROR("Check both %s.",
                          (isFunc ? "arguments" : "operands"));
                } else {
                    ERROR("Check %s %s.",
                          (left ? "left" : "right"),
                          (isFunc ? "argument" : "operand"));
                }
            }
            exit(-1);
        }
    }
}

double complex _umns(double complex a, Expression *e) {
    return -a;
}

double complex _flip(double complex a, Expression *e) {
    if (!EQI(a, 0)) {
        return creal(a) - cimag(a) * I;
    } else {
        int n = (int) a;

        int p = 0;
        while ((1 << p) < n) ++p;

        return (~n) & ((1 << p) - 1);
    }
}

double complex _fact(double complex a, Expression *e) {
    numberException(a, 0, e, "!", COMPLEX_AND_DOUBLE, 0, 1);
    if ((int) a < 0) {
        throwException("Operation '!' is not defined for negative numbers.", e);
    }
    double complex res = 1;
    for (int i = 2; i <= (int) a; ++i) {
        res *= i;
    }
    return res;
}

double complex _sum(double complex a, double complex b, Expression *e) {
    return a + b;
}

double complex _sub(double complex a, double complex b, Expression *e) {
    return a - b;
}

double complex _mul(double complex a, double complex b, Expression *e) {
    return a * b;
}

double complex _div(double complex a, double complex b, Expression *e) {
    if (EQC(b, 0)) {
        throwException("Division by zero.", e);
    }
    return a / b;
}

double complex _mod(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "%", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a % (int) b;
}

double complex _pwr(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "^", COMPLEX, 0, 2);
    if (EQR(a, 0) && creal(b) < 0) {
        throwException("Operation '^' is not defined for negative powers of zero.", e);
    }
    return pow(a, b);
}

double complex _and(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "&", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a & (int) b;
}

double complex _or(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "|", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a | (int) b;
}

double complex _xor(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "@", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a ^ (int) b;
}

double complex _sin(double complex a, Expression *e) {
    return csin(a);
}

double complex _cos(double complex a, Expression *e) {
    return ccos(a);
}

double complex _tg(double complex a, Expression *e) {
    if (EQC(_cos(a, e), 0)) {
        throwException("Function 'tg()' is not defined for phase pi/2 + pi*k.", e);
    }
    return _sin(a, e) / _cos(a, e);
}

double complex _ctg(double complex a, Expression *e) {
    if (EQC(_sin(a, e), 0)) {
        throwException("Function 'ctg()' is not defined for phase pi*k.", e);
    }
    return _cos(a, e) / _sin(a, e);
}

double complex _rad(double complex a, Expression *e) {
    numberException(a, 0, e, "rad()", COMPLEX, 1, 1);
    return a / 180 * _pi();
}

double complex _floor(double complex a, Expression *e) {
    numberException(a, 0, e, "floor()", COMPLEX, 1, 1);
    return floor(a);
}

double complex _ceil(double complex a, Expression *e) {
    numberException(a, 0, e, "ceil()", COMPLEX, 1, 1);
    return ceil(a);
}

double complex _ln(double complex a, Expression *e) {
    if (EQI(a, 0) && creal(a) < 0) {
        throwException("Function 'ln()' for non-complex numbers defined for positive arguments.", e);
    }
    return clog(a);
}

double complex _log(double complex a, Expression *e) {
    if (EQI(a, 0) && creal(a) < 0) {
        throwException("Function 'log()' for non-complex numbers defined for positive arguments.", e);
    }
    return clog10(a);
}

double complex _sqrt(double complex a, Expression *e) {
    if (EQI(a, 0) && creal(a) < 0 && !EQR(a, -1)) {
        throwException("Function 'sqrt()' for non-complex numbers defined only for positive arguments.", e);
    }
    return csqrt(a);
}

double complex _abs(double complex a, Expression *e) {
    numberException(a, 0, e, "abs()", COMPLEX, 1, 1);
    return fabs(creal(a));
}

double complex _exp(double complex a, Expression *e) {
    return cexp(a);
}

double complex _real(double complex a, Expression *e) {
    return creal(a);
}

double complex _imag(double complex a, Expression *e) {
    return cimag(a);
}

double complex _mag(double complex a, Expression *e) {
    return cabs(a);
}

double complex _phase(double complex a, Expression *e) {
    return carg(a);
}

double complex _pow(double complex a, double complex b, Expression *e) {
    if (EQR(a, 0) && EQI(a, 0) && creal(b) < 0 && EQI(b, 0)) {
        throwException("Function 'pow()' is not defined for negative powers of zero", e);
    }
    return cpow(a, b);
}

double complex _min(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "min()", COMPLEX, 1, 2);
    return (creal(a) < creal(b) ? a : b);
}

double complex _max(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "max()", COMPLEX, 1, 2);
    return (creal(a) > creal(b) ? a : b);
}

double complex _rand(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "rand()", COMPLEX_AND_DOUBLE, 1, 2);
    return (int) a + rand() % ((int) b - (int) a + 1);
}

double complex _pi() {
    return M_PI;
}

double complex _euler() {
    return M_E;
}

double complex _j() {
    return I;
}