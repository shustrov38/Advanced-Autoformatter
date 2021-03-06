#include "ops.h"

OpID __getOpID(char *op) {
    // extra options
    if (!strcmp(op, "++"))
        return NUM;
    if (!strcmp(op, "--"))
        return NUM;
    if (!strcmp(op, "+"))
        return NUM;
    if (!strcmp(op, "-"))
        return NUM;

    if (!strcmp(op, "="))
        return EQL;

    if (!strcmp(op, "("))
        return OPB;

    if (!strcmp(op, ")"))
        return CLB;

    if (!strcmp(op, ","))
        return COM;

    if (!strcmp(op, ";"))
        return COM;

    if ('0' <= op[0] && op[0] <= '9')
        return NUM;

    // arithmetic operations
    if (!strcmp(op, "*"))
        return MUL;

    if (!strcmp(op, "/"))
        return DIV;

    if (!strcmp(op, "%"))
        return MOD;

    if (!strcmp(op, "!"))
        return LNOT;

    if (!strcmp(op, "&&"))
        return LAND;

    if (!strcmp(op, "||"))
        return LOR;

    if (!strcmp(op, "^"))
        return BXOR;

    if (!strcmp(op, "&"))
        return BAND;

    if (!strcmp(op, "|"))
        return BOR;

    if (!strcmp(op, "~"))
        return FLIP;

    if (!strcmp(op, "<<"))
        return SHL;

    if (!strcmp(op, ">>"))
        return SHR;

    if (!strcmp(op, "<"))
        return CMPL;

    if (!strcmp(op, "<="))
        return CMPLE;

    if (!strcmp(op, ">"))
        return CMPG;

    if (!strcmp(op, ">="))
        return CMPGE;

    if (!strcmp(op, "=="))
        return EQLS;

    if (!strcmp(op, "!="))
        return CMPNE;

    // functions
    if (!strcmp(op, "sin"))
        return SIN;

    if (!strcmp(op, "cos"))
        return COS;

    if (!strcmp(op, "tg"))
        return TG;

    if (!strcmp(op, "ctg"))
        return CTG;

    if (!strcmp(op, "floor"))
        return FLR;

    if (!strcmp(op, "ceil"))
        return CEIL;

    if (!strcmp(op, "log"))
        return LOG;

    if (!strcmp(op, "sqrt"))
        return SQRT;

    if (!strcmp(op, "pow"))
        return POW;

    if (!strcmp(op, "abs"))
        return ABS;

    if (!strcmp(op, "exp"))
        return EXP;

    // standard variable
    return VAR;
}

OpInfo *getLineOfIDs(char **code, int size) {
    OpInfo *res = (OpInfo *) malloc(size * sizeof(OpInfo));

    for (int i = 0; i < size; ++i) {
        if (!strcmp(code[i], "-")) {
            if (i == 0 || res[i - 1].id == OPB) {
                res[i].id = UMNS;
            } else {
                res[i].id = MNS;
            }
        }

        else if (!strcmp(code[i], "--")) {
            if (i + 1 != size && (__getOpID(code[i+1]) == VAR || __getOpID(code[i+1]) == OPB)) {
                res[i].id = PREF_DEC;
                for (int j = i; j < size; ++j){
                    if(__getOpID(code[j]) == VAR){
                        strcpy(res[i].relatedTo,code[j]);
                        break;
                    }
                }
            } else {
                res[i].id = POST_DEC;
                for (int j = i; j >= 0; --j){
                    if(res[j].id == VAR){
                        strcpy(res[i].relatedTo,code[j]);
                        break;
                    }
                }
            }
        }

        else if (!strcmp(code[i], "+")) {
            if (i == 0 || res[i - 1].id == OPB) {
                res[i].id = UPLS;
            } else {
                res[i].id = PLS;
            }
        }

        else if (!strcmp(code[i], "++")) {
            if (i + 1 != size && (__getOpID(code[i+1]) == VAR || __getOpID(code[i+1]) == OPB)) {
                res[i].id = PREF_INC;
                for (int j = i; j < size; ++j){
                    if(__getOpID(code[j]) == VAR){
                        strcpy(res[i].relatedTo,code[j]);
                        break;
                    }
                }
            } else {
                res[i].id = POST_INC;
                for (int j = i; j >= 0; --j){
                    if(res[j].id == VAR){
                        strcpy(res[i].relatedTo,code[j]);
                        break;
                    }
                }
            }
        }

        else {
            res[i].id = __getOpID(code[i]);
        }
    }

    return res;
}

Priority getOpPriority(OpID id) {
    switch (id) {
        case EQL:
            return WORST;
        case SHR:
        case SHL:
        case EQLS:
            return POSTFIX;
        case CMPL:
        case CMPLE:
        case CMPG:
        case CMPGE:
        case CMPNE:
            return COMPARE;
        case POST_INC:
        case POST_DEC:
        case PLS:
        case MNS:
        case BOR:
        case BXOR:
        case LOR:
            return SUM;
        case MUL:
        case DIV:
        case MOD:
        case BAND:
        case LAND:
            return PROD;
        case SIN:
        case COS:
        case TG:
        case CTG:
        case FLR:
        case CEIL:
        case LOG:
        case SQRT:
        case POW:
        case ABS:
        case EXP:
            return FUNC;
        case PWR:
            return POWER;
        case LNOT:
        case UMNS:
        case UPLS:
        case FLIP:
        case PREF_INC:
        case PREF_DEC:
            return PREFIX;
        default:
            return NONE;
    }
}