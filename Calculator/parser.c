#include "parser.h"

#define MAX_ARRAY_SIZE 100
#define MAX_E_SIZE 100
#define MAX_STR_SIZE 100

#define ERROR(...) fprintf(stderr, __VA_ARGS__)

void initExpression(Expression *E) {
    for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
        E[i].code = (char **) malloc(MAX_E_SIZE * sizeof(char *));
        for (int j = 0; j < MAX_ARRAY_SIZE; ++j) {
            E[i].code[j] = (char *) malloc(MAX_V_NAME_SIZE * sizeof(char));
            memset(E[i].code[j], 0, MAX_V_NAME_SIZE);
        }
    }
}

Expression *createExpressions() {
    Expression *tmp = (Expression *) malloc(MAX_ARRAY_SIZE * sizeof(Expression));
    assert((tmp) && "null ptr at creating expressions array");
    initExpression(tmp);
    return tmp;
}

int addExpression(Expression *expr, int exprSize, char **src, int srcSize, Stack *metaData, int metaVal) {
    int sizeDelta = 1;
    // check for ';' at the end of code line
    if (strcmp(src[0], "for")!=0) {
        for (int j = 0; j < srcSize; ++j) {
            if (!strcmp(src[j], ";")) {
                srcSize = j;
                break;
            }
        }
    } else if (!strcmp(src[0], "for")){
        for (int j = 0; j < srcSize; ++j) {
            if (!strcmp(src[j], "{")) {
                srcSize = j;
                break;
            }
        }
    }

    int addBracket = 0;
    int exprInd = 0;
    int i = 0;
    if (srcSize > 1 && strcmp(src[1], "==") != 0 && strcmp(src[1], "=") != 0 && src[1][strlen(src[1]) - 1] == '=') {
        src[1][strlen(src[1]) - 1] = 0;
        strcpy(expr[exprSize].code[exprInd++], src[0]);
        strcpy(expr[exprSize].code[exprInd++], "=");
        if (strlen(src[1]) > 0) {
            strcpy(expr[exprSize].code[exprInd++], src[0]);
            strcpy(expr[exprSize].code[exprInd++], src[1]);
            strcpy(expr[exprSize].code[exprInd++], "(");
            addBracket = 1;
        }
        i = 2;
    } else if (!strcmp(src[0], "if") || !strcmp(src[0], "while")) {
        strcpy(expr[exprSize].code[exprInd], src[0]);
        char *metaStr = (char *) malloc(5 * sizeof(char));
        sprintf(metaStr, "%d", metaVal);
        strcat(expr[exprSize].code[exprInd++], metaStr);
        stData tmp = {.str = expr[exprSize].code[exprInd - 1]};
        stPush(metaData, tmp);
        strcpy(expr[exprSize].code[exprInd++], "=");
        strcpy(expr[exprSize].code[exprInd++], "(");
        addBracket = 1;
        i = 1;
    }else if (!strcmp(src[0], "for")) {
        srcSize-=2;
        i = 2;
        char **forInit = (char **)malloc(10*sizeof (char*));
        for (int y = 0; y < 10; y++){
            forInit[y] = (char*) malloc(10*sizeof (char));
            memset(forInit[y],0,10);
        }
        for(int yy = 0;strcmp(src[i],";");){
            strcpy(forInit[yy++],src[i++]);
        }
        addExpression(expr,exprSize++,forInit,i-2,NULL,0);
        sizeDelta++;
        i++;

        char **forCond = (char **)malloc(10*sizeof (char*));
        for (int y = 0; y < 10; y++){
            forCond[y] = (char*) malloc(10*sizeof (char));
            memset(forInit[y],0,10);
        }
        int forCondIdx = 0;
        strcpy(forCond[forCondIdx], src[0]);
        char *metaStr = (char *) malloc(5 * sizeof(char));
        sprintf(metaStr, "%d", metaVal);
        strcat(forCond[forCondIdx++], metaStr);
        stData tmp = {.str = forCond[forCondIdx-1]};
        stPush(metaData, tmp);
        strcpy(forCond[forCondIdx++], "=");
        strcpy(forCond[forCondIdx++], "(");
        for(;strcmp(src[i],";");){
            strcpy(forCond[forCondIdx++],src[i++]);
        }
        strcpy(forCond[forCondIdx++],")");
        addExpression(expr,exprSize++,forCond,forCondIdx,NULL,0);
        sizeDelta++;
        i++;

    } else if (!strcmp(src[0], "}")) {
        sprintf(expr[exprSize].code[exprInd++],"%s end",stTop(metaData).str);
        //strcpy(expr[exprSize].code[exprInd++], stTop(metaData).str);
        stPop(metaData);
        i = 1;
    }

    for (; i < srcSize ; ++i) {
        strcpy(expr[exprSize].code[exprInd++], src[i]);
    }

    if (addBracket) {
        strcpy(expr[exprSize].code[exprInd++], ")");
    }

//    printf("\n%d", srcSize);
    expr[exprSize].size = exprInd;
//    for (int u = 0; u < exprInd; u++) {
//        printf(" %s", expr[exprSize].code[u]);
//    }
    return sizeDelta;
}

void destroyExpressionsArray(Expression *E) {
    for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
        for (int j = 0; j < MAX_E_SIZE; ++j) {
            free(E[i].code[j]);
        }
        free(E[i].code);
    }
}