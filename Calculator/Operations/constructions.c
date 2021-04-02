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

double complex idToFunction(stData *data, Expression *e, int ind, int n, double complex a, double complex b) {
    switch (data->info.id) {
        case POST_DEC:
            return _post_dec(b, &e[ind]);
        case POST_INC:
            return _post_inc(b, &e[ind]);
        case PREF_DEC:
            return _pref_dec(b, &e[ind]);
        case PREF_INC:
            return _pref_inc(b, &e[ind]);
        case UMNS:
            return _umns(b, &e[ind]);
        case UPLS:
            return _upls(b, &e[ind]);
        case FLIP:
            return _flip(b, &e[ind]);
        case LNOT:
            return _lnot(b, &e[ind]);
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
        case BAND:
            return _band(a, b, &e[ind]);
        case BOR:
            return _bor(a, b, &e[ind]);
        case BXOR:
            return _bxor(a, b, &e[ind]);
        case LAND:
            return _land(a, b, &e[ind]);
        case LOR:
            return _lor(a, b, &e[ind]);
        case SHL:
            return _shl(a, b, &e[ind]);
        case SHR:
            return _shr(a, b, &e[ind]);
        case CMPL:
            return _cmpl(a, b, &e[ind]);
        case CMPLE:
            return _cmple(a, b, &e[ind]);
        case CMPG:
            return _cmpg(a, b, &e[ind]);
        case CMPGE:
            return _cmpge(a, b, &e[ind]);
        case EQLS:
            return _eqls(a, b, &e[ind]);
        case SIN:
            return _sin(b, &e[ind]);
        case COS:
            return _cos(b, &e[ind]);
        case TG:
            return _tg(b, &e[ind]);
        case CTG:
            return _ctg(b, &e[ind]);
        case FLR:
            return _floor(b, &e[ind]);
        case CEIL:
            return _ceil(b, &e[ind]);
        case LOG:
            return _log(b, &e[ind]);
        case SQRT:
            return _sqrt(b, &e[ind]);
        case ABS:
            return _abs(b, &e[ind]);
        case EXP:
            return _exp(b, &e[ind]);
        case POW:
            return _pow(a, b, &e[ind]);
        case VAR:
            // todo: mem access
            return 0;
        default:
            return toComplex(data->str);
    }
}

void throwException(char *err, Expression *e) {
    ERROR("%s", err);
    exit(-1);
}

// type 0 - complex, 1 - double, 2 - complex and double;
void numberException(double complex a, double complex b, Expression *e, char *symbol, int type, int isFunc, int args) {
    1;
}

double complex _post_dec(double complex a, Expression *e) {
    numberException(a, 0, e, "--", COMPLEX_AND_DOUBLE, 0, 1);
    return creal(a) - 1;
}

double complex _post_inc(double complex a, Expression *e) {
    numberException(a, 0, e, "++", COMPLEX_AND_DOUBLE, 0, 1);
    return creal(a) + 1;
}

double complex _pref_dec(double complex a, Expression *e) {
    numberException(a, 0, e, "--", COMPLEX_AND_DOUBLE, 0, 1);
    return creal(a) - 1;
}

double complex _pref_inc(double complex a, Expression *e) {
    numberException(a, 0, e, "++", COMPLEX_AND_DOUBLE, 0, 1);
    return creal(a) + 1;
}

double complex _umns(double complex a, Expression *e) {
    return -a;
}

double complex _upls(double complex a, Expression *e) {
    return a;
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

double complex _band(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "&", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a & (int) b;
}

double complex _bor(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "|", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a | (int) b;
}

double complex _bxor(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "^", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a ^ (int) b;
}

double complex _land(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "&&", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a && (int) b;
}

double complex _lor(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "||", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a || (int) b;
}

double complex _lnot(double complex a, Expression *e) {
    numberException(a, 0, e, "!", COMPLEX_AND_DOUBLE, 0, 1);
    return !(int) a;
}

double complex _shl(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "<<", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a << (int) b;
}

double complex _shr(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, ">>", COMPLEX_AND_DOUBLE, 0, 2);
    return (int) a >> (int) b;
}

double complex _cmpl(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "<", COMPLEX, 0, 2);
    return creal(a) < creal(b);
}

double complex _cmple(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "<=", COMPLEX, 0, 2);
    return creal(a) >= creal(b);
}

double complex _cmpg(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, ">", COMPLEX, 0, 2);
    return creal(a) > creal(b);
}

double complex _cmpge(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, ">=", COMPLEX, 0, 2);
    return creal(a) >= creal(b);
}

double complex _eqls(double complex a, double complex b, Expression *e) {
    numberException(a, b, e, "==", COMPLEX, 0, 2);
    return creal(a) == creal(b);
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

double complex _floor(double complex a, Expression *e) {
    numberException(a, 0, e, "floor()", COMPLEX, 1, 1);
    return floor(a);
}

double complex _ceil(double complex a, Expression *e) {
    numberException(a, 0, e, "ceil()", COMPLEX, 1, 1);
    return ceil(a);
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

double complex _pow(double complex a, double complex b, Expression *e) {
    if (EQR(a, 0) && EQI(a, 0) && creal(b) < 0 && EQI(b, 0)) {
        throwException("Function 'pow()' is not defined for negative powers of zero", e);
    }
    return cpow(a, b);
}