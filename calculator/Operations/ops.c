#include "ops.h"

OpID getOpID(char *op) {
    // extra options
    if (!strcmp(op, "="))
        return PLS;

    if (!strcmp(op, "("))
        return OPB;

    if (!strcmp(op, ")"))
        return CLB;

    if (!strcmp(op, ","))
        return COM;

    if ('0' <= op[0] && op[0] <= '9')
        return NUM;

    // arithmetic operations
    if (!strcmp(op, "--"))
        return UMNS;

    if (!strcmp(op, "~"))
        return FLIP;

    if (!strcmp(op, "!"))
        return FACT;

    if (!strcmp(op, "+"))
        return PLS;

    if (!strcmp(op, "-"))
        return MNS;

    if (!strcmp(op, "*"))
        return MUL;

    if (!strcmp(op, "/"))
        return DIV;

    if (!strcmp(op, "%"))
        return MOD;

    if (!strcmp(op, "^"))
        return PWR;

    if (!strcmp(op, "&"))
        return AND;

    if (!strcmp(op, "|"))
        return OR;

    if (!strcmp(op, "@"))
        return XOR;

    // functions
    if (!strcmp(op, "sin"))
        return SIN;

    if (!strcmp(op, "cos"))
        return COS;

    if (!strcmp(op, "tg"))
        return TG;

    if (!strcmp(op, "ctg"))
        return CTG;

    if (!strcmp(op, "radians"))
        return RAD;

    if (!strcmp(op, "floor"))
        return FLR;

    if (!strcmp(op, "ceil"))
        return CEIL;

    if (!strcmp(op, "log"))
        return LOG;

    if (!strcmp(op, "ln"))
        return LN;

    if (!strcmp(op, "sqrt"))
        return SQRT;

    if (!strcmp(op, "pow"))
        return POW;

    if (!strcmp(op, "abs"))
        return ABS;

    if (!strcmp(op, "exp"))
        return EXP;

    if (!strcmp(op, "real"))
        return REAL;

    if (!strcmp(op, "imag"))
        return IMAG;

    if (!strcmp(op, "mag"))
        return MAG;

    if (!strcmp(op, "phase"))
        return PHASE;

    if (!strcmp(op, "min"))
        return MIN;

    if (!strcmp(op, "max"))
        return MAX;

    if (!strcmp(op, "rand"))
        return RND;

    // constants
    if (!strcmp(op, "pi"))
        return PI;

    if (!strcmp(op, "e"))
        return EULER;

    if (!strcmp(op, "j"))
        return J;

    // standard variable
    return VAR;
}

Priority getOpPriority(OpID id) {
    switch (id) {
        case PLS:
        case MNS:
        case OR:
        case XOR:
            return SUM;
        case MUL:
        case DIV:
        case MOD:
        case AND:
            return PROD;
        case SIN:
        case COS:
        case TG:
        case CTG:
        case RAD:
        case FLR:
        case CEIL:
        case LOG:
        case LN:
        case SQRT:
        case POW:
        case ABS:
        case EXP:
        case REAL:
        case IMAG:
        case MAG:
        case PHASE:
        case MIN:
        case MAX:
        case RND:
            return FUNC;
        case PWR:
            return POWER;
        case UMNS:
        case FLIP:
            return UNARY;
        case FACT:
            return FACTORIAL;
        default:
            return NONE;
    }
}