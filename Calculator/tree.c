#include "tree.h"

Node *nodeInit() {
    Node *node = (Node *) malloc(sizeof(Node));
    assert(node != NULL && "bad mem allocate");
    node->elementSize = sizeof(stData);
    node->left = node->right = NULL;
    return node;
}

void opTreeGen(Node *node, Stack *stack) {
    assert(node != NULL && "bad root");
    assert(stack != NULL && stack->size != 0 && "bad stack");

    // recursive end condition
    if (stack->size == 0) return;

    stData temp = stTop(stack);
    memcpy(&node->value, &temp, sizeof(stData));
    stPop(stack);

    if (IS_OPER(node->value.info.id) || IS_UOPER(node->value.info.id)) {
        node->state = OPERATION;
    } else if (IS_FUNC_1ARG(node->value.info.id)) {
        node->state = FUNCTION1;
    } else if (IS_FUNC_2ARG(node->value.info.id)) {
        node->state = FUNCTION2;
    } else /* var or const */ {
        node->state = BASIC;
    }

    if (node->state != BASIC) {
        node->right = nodeInit();
        opTreeGen(node->right, stack);
        // unary and func1 require one argument
        if (node->state != FUNCTION1 && !IS_UOPER(node->value.info.id)) {
            node->left = nodeInit();
            opTreeGen(node->left, stack);
        }
    }
}

double opTreeCalc(Memory *m, Node *root) {
    if (root == NULL) return 0;
    double l = (opTreeCalc(m, root->left));
    double r = (opTreeCalc(m, root->right));

    Variant *var;

    switch (root->value.info.id) {
        case EQL:
            var = MemoryFunctions.getValue(m, root->left->value.str);
            if (var->numCap == Int) {
                var->d = r;
                var->numCap = Double;
            }
            else if (var->type == String) { var->d = r; }
            return 0;
        case NUM:
            return atof(root->value.str);
        case UMNS:
            return -r;
        case UPLS:
            return +r;
        case FLIP:
            return ~(int) r;
        case PREF_INC:
            MemoryFunctions.inc(m, root->value.info.relatedTo);
            return r + 1;
        case POST_INC:
            MemoryFunctions.inc(m, root->value.info.relatedTo);
            return r;
        case PREF_DEC:
            MemoryFunctions.dec(m, root->value.info.relatedTo);
            return r - 1;
        case POST_DEC:
            MemoryFunctions.dec(m, root->value.info.relatedTo);
            return r;
        case PLS:
            return r + l;
        case MNS:
            return r - l;
        case MUL:
            return r * l;
        case DIV:
            return r / l;
        case MOD:
            return (int) r % (int) l;
        case BAND:
            return (int) r & (int) l;
        case BOR:
            return (int) r | (int) l;
        case BXOR:
            return (int) r ^ (int) l;
        case LAND:
            return (int) r && (int) l;
        case LOR:
            return (int) r || (int) l;
        case LNOT:
            return !(int) r;
        case SHR:
            return (int) r >> (int) l;
        case SHL:
            return (int) r << (int) l;
        case CMPL:
            return r < l;
        case CMPLE:
            return r <= l;
        case CMPG:
            return r > l;
        case CMPGE:
            return r >= l;
        case EQLS:
            return r == l;
        case SIN:
            return sin(r);
        case COS:
            return cos(r);
        case TG:
            return tan(r);
        case CTG:
            return ctan(r);
        case FLR:
            return floor(r);
        case CEIL:
            return ceil(r);
        case LOG:
            return log(r);
        case SQRT:
            return sqrt(r);
        case ABS:
            return fabs(r);
        case EXP:
            return exp(r);
        case POW:
            return pow(r, l);
        case VAR:
            var = MemoryFunctions.getValue(m, root->value.str);
            if (var->type == Double) { return var->d; }
            else if (var->type == Int) { return (double) var->d; }
            else if (var->type == String) { return 0; }
        default:
            return 0;
    }
}