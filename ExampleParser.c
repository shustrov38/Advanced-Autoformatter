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
        E[i].dependencies = (char **) malloc(MAX_ARRAY_SIZE * sizeof(char *));
        for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
            E[i].formula[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].formula[j], 0, MAX_V_NAME_SIZE);
            E[i].dependencies[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].dependencies[j], 0, MAX_V_NAME_SIZE);
        }
        E[i].varName = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
        E[i].evenDependenciesCnt = 0;
        E[i].trueDependenciesCnt = 0;

        for (int j = 0; j < MAX_E_SIZE; ++j) {
            memset(E[i].formula[j], 0, MAX_V_NAME_SIZE);
            memset(E[i].dependencies[j], 0, MAX_V_NAME_SIZE);
        }

    }
}

Expression *createExpressions() {
    Expression *tmp = (Expression *) malloc(MAX_ARRAY_SIZE * sizeof(Expression));
    assert((tmp) && "null ptr at creating expressions array");
    initExpression(tmp);
    return tmp;
}

int splitExpression(char *src, char **dest, char divs[]) {
    assert((src) && "given null str ptr");

    int lastValCh = (int) strlen(src) - 1;
    while (src[lastValCh] == ' ' || src[lastValCh] == '\n' || src[lastValCh] == '\t') src[lastValCh--] = '\0';

    char tmpStr[MAX_ARRAY_SIZE][MAX_V_NAME_SIZE];
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        memset(tmpStr[i], 0, MAX_V_NAME_SIZE);
    }
    int i = 0;
    int k = 0;
    int z = 0;
    int opF = 1; //is op flag for ch
    for (int ff = 0; ff < strlen(src); ff++) {
        if (src[ff] == '#') {
            src[ff] = '\0';
            break;
        }
    }
    while (src[i] != '\n' && src[i] != '\0' && src[i] != '\r') {
        while (i < strlen(src) - 1 && (src[i] == ' ' || src[i] == '\t')) i++;
        int dvF = 0;
        for (int j = 0; j < strlen(divs); j++) {
            if (src[i] == divs[j]) dvF = 1;
        }
        if (!dvF) {
            tmpStr[k][z++] = src[i++];
            opF = 0;
        } else {
            z = 0;
            if (!opF) k++;
            tmpStr[k][0] = src[i];
            // check for unary minus
            if (src[i] == '-') {
                if (i == 0 || tmpStr[k - 1][0] == '=' || tmpStr[k - 1][0] == '(' || tmpStr[k - 1][0] == ',') {
                    tmpStr[k][1] = '-';
                }
            }
            // end check
            ++k;
            ++i;
            opF = 1;
        }
    }

    for (i = 0; i < MAX_ARRAY_SIZE; ++i) {
        strcpy(dest[i], tmpStr[i]);
    }

    while ((dest[k][0] == ' ' || dest[k][0] == '\t')) k--;

    return k + (dest[k][0] != 0);
}

void checkForErrors(char **dest, int dlenght, char *rawForm) {
    int brCnt = 0;
    for (int i = 0; i < dlenght; ++i) {
        if (getOpID(dest[i]) == OPB) {
            brCnt++;

        } else if (getOpID(dest[i]) == CLB) {
            brCnt--;
            if (i > 0 && getOpID(dest[i - 1]) == OPB) {
                printPseudoStr(rawForm);
                ERROR("BAD EXPRESSION NOTATION: empty brackets");
                exit(-1);
            }
        }
        if (brCnt < 0) {
            printPseudoStr(rawForm);
            ERROR("BAD EXPRESSION NOTATION: wrong bracket sequence");
            exit(-1);
        }
        if (getOpID(dest[i]) == COM) {
            if (i == 0 || i == dlenght - 1 || getOpID(dest[i - 1]) == OPB || getOpID(dest[i + 1]) == CLB) {
                printPseudoStr(rawForm);
                ERROR("BAD EXPRESSION NOTATION: wrong place for ','");
                exit(-1);
            }
        }
        if (IS_OPER(dest[i])) { // check for {binary operand} exception
            if (i == 0 || IS_OPER(dest[i - 1]) || IS_OPER(dest[i + 1]) ||
                (getOpID(dest[i - 1]) == OPB || getOpID(dest[i + 1]) == CLB)) {
                printPseudoStr(rawForm);
                ERROR("BAD EXPRESSION NOTATION: operator '%s' must have two correct operands", dest[i]);
                exit(-1);
            }
        } else if (IS_NUM(dest[i])) { // check for {num vals} exception
            int pointCnt = 0;
            if (dest[i][0] == '0' && dest[i][1] != '\0' && dest[i][1] != '.') {
                printPseudoStr(rawForm);
                ERROR("BAD NUMBER: wrong number input");
                exit(-1);
            }
            for (int j = 0; dest[i][j] != '\0'; j++) {
                if (dest[i][j] == '.') {
                    pointCnt++;
                    if (dest[i][j + 1] == '\0') {
                        printPseudoStr(rawForm);
                        ERROR("BAD NUMBER: wrong float value notation");
                        exit(-1);
                    }
                    if (j > 0 && pointCnt > 1) {
                        printPseudoStr(rawForm);
                        ERROR("BAD NUMBER: wrong float value notation");
                        exit(-1);
                    }
                } else if (!(dest[i][j] >= '0' && dest[i][j] <= '9' || dest[i][j] == '.' ||
                             dest[i][j] == 'j')) { // appropriate char check
                    printPseudoStr(rawForm);
                    ERROR("BAD NUMBER: wrong number char");
                    exit(-1);
                } else if (dest[i][j] == 'j' && dest[i][j + 1] != '\0') {
                    printPseudoStr(rawForm);
                    ERROR("BAD NUMBER: wrong complex value notation");
                    exit(-1);
                }
            }
        } else if (IS_VAR(dest[i])) { // check for {var names} exception
            if (!((dest[i][0] >= 'a' && dest[i][0] <= 'z') || (dest[i][0] == '_'))) {
                printPseudoStr(rawForm);
                ERROR("BAD VAR NAME: explicit char in variable name");
                exit(-1);
            }
            for (int j = 1; dest[i][j] != '\0'; j++) {
                if (!((dest[i][j] >= 'a' && dest[i][j] <= 'z') || (dest[i][j] >= '0' && dest[i][j] <= '9') ||
                      (dest[i][j] == '_'))) {
                    printPseudoStr(rawForm);
                    ERROR("BAD VAR NAME: explicit char in variable name");
                    exit(-1);
                }
            }
        } else if (IS_FUNC_1ARG(dest[i])) { // check for {1 arg func} exception
            int countBr = 1;
            for (int j = i + 2; countBr != 0 && j < dlenght; j++) {
                if (getOpID(dest[j]) == OPB) {
                    countBr++;
                } else if (getOpID(dest[j]) == CLB) {
                    countBr--;
                }
                if (getOpID(dest[j]) == COM && countBr == 1) {
                    printPseudoStr(rawForm);
                    ERROR("FUNCTION %s ERROR: required 1 argument but more given", dest[i]);
                    exit(-1);
                }
            }
        } else if (IS_FUNC_2ARG(dest[i])) { // need to fix ','
            int countBr = 1;
            int countCom = 0;
            for (int j = i + 2; countBr != 0 && j < dlenght; j++) {
                if (getOpID(dest[j]) == OPB) {
                    countBr++;
                } else if (getOpID(dest[j]) == CLB) {
                    countBr--;
                }
                if (getOpID(dest[j]) == COM && countBr == 1) {
                    countCom++;
                }
                if (countCom == 2) {
                    printPseudoStr(rawForm);
                    ERROR("FUNCTION %s ERROR: required 2 arguments", dest[i]);
                    exit(-1);
                }
            }
            if (countCom != 1) {
                printPseudoStr(rawForm);
                ERROR("FUNCTION %s ERROR: required 2 arguments", dest[i]);
                exit(-1);
            }
        }
    }
    if (brCnt != 0) {
        printPseudoStr(rawForm);
        ERROR("BAD EXPRESSION NOTATION: wrong bracket sequence");
        exit(-1);
    }
}

int parserReadExpressions(char *filename, Expression *e) {
    FILE *in = fopen(filename, "r");
    if (in == NULL) {
        ERROR("FILE PROBLEM: Can't open input file.\n");
        exit(-1);
    }
    assert((e) && "null ptr of struct");
    char buffStr[MAX_E_SIZE];
    int number = 0;
    while (fgets(buffStr, MAX_E_SIZE, in)) {
        // comment
        int buffLen = (int) strlen(buffStr);
        for (int i = 0; i < buffLen; ++i) {
            if (buffStr[i] == '#') {
                buffStr[i] = '\0';
                break;
            }
        }
        buffLen = (int) strlen(buffStr) - 1;
        while (buffStr[buffLen] == ' ' || buffStr[buffLen] == '\t') buffLen--;
        buffStr[buffLen + 1] = '\0';
        int idx = 0;
        while (buffStr[idx] == ' ' || buffStr[idx] == '\t') idx++;
        if (buffStr[idx] == '\0' || buffStr[idx] == '\n') {
            continue;
        }
        // end comment
        // raw
        strcpy(e[number].rawFormula, buffStr);
        if (e[number].rawFormula[strlen(e[number].rawFormula) - 1] == '\n') {
            e[number].rawFormula[strlen(e[number].rawFormula) - 1] = 0;
        }
        // end raw
        for (int i = 0; i < strlen(buffStr); ++i) {
            if (buffStr[i] >= 'A' && buffStr[i] <= 'Z') buffStr[i] += ('a' - 'A');
        }
        e[number].segCnt = splitExpression(buffStr, e[number].formula, "()=-+*^/,%&|@~!");
        if (e[number].formula[0] && !strcmp(e[number].formula[1], "=")) {
            strcpy(e[number].varName, e[number].formula[0]);
            for (int segI = 0; segI < e[number].segCnt - 2; segI++) {
                strcpy(e[number].formula[segI], e[number].formula[segI + 2]);
            }
            memset(e[number].formula[e[number].segCnt - 1], 0, MAX_V_NAME_SIZE);
            memset(e[number].formula[e[number].segCnt - 2], 0, MAX_V_NAME_SIZE);
            e[number].segCnt -= 2;
        }
        int i = 0;
        int j = 0;
        while (e[number].formula[i][0] != '\0') {
            assert(getOpID(e[number].formula[i]) && "null opId");
            if (getOpID(e[number].formula[i]) == VAR) {
                int uniqDep = 1;
                for (int l = 0; l < e[number].evenDependenciesCnt; l++) {
                    if (!strcmp(e[number].formula[i], e[number].dependencies[l])) uniqDep = 0;
                }
                if (uniqDep) {
                    strcpy(e[number].dependencies[j++], e[number].formula[i]);
                    assert(e[number].dependencies[j - 1] && "null str after strcpy at building dependencies");
                    e[number].evenDependenciesCnt++;
                }
            }
            i++;
        }
#ifdef __PARSER_DEBUG__
        printf("expression (%s) #%d:[ ", e[number].varName, number + 1);
        for (i = 0; i < MAX_E_SIZE && e[number].formula[i][0] != '\0'; ++i) {
            printf("%s ", e[number].formula[i]);
        }
        printf("] ");
        if (e[number].evenDependenciesCnt > 0) {
            printf("dependencies are: ");
            for (i = 0; i < e[number].evenDependenciesCnt; ++i) {
                printf("'%s' ", e[number].dependencies[i]);
            }
        }
        printf("\n");
#endif //__PARSER_DEBUG__
        checkForErrors(e[number].formula, e[number].segCnt, e[number].rawFormula);
        ++number;
    }
    fclose(in);
    return number;
}

void destroyExpressionsArray(Expression *E) {
    assert((E) && "null ptr, lul, nothing to delete");
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        for (int j = 0; j < MAX_E_SIZE; ++j) {
            free(E[i].formula[j]);
            free(E[i].dependencies[j]);
        }
        free(E[i].varName);
        free(E[i].formula);
        free(E[i].dependencies);
    }
    free(E);
}