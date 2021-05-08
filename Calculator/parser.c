#include "parser.h"
#include "../vector.h"

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

int addExpression(Expression *expr, int exprSize, char **src, int srcSize, Stack *metaData, int metaVal,vector *exe,
                  vector *reqSize, bool *boolStack, int *bcnt) {
    int sizeDelta = 1;
    // check for ';' at the end of code line
    if (!strcmp(src[0], "for") || !strcmp(src[0], "if") || !strcmp(src[0], "while")) {
        for (int j = 0; j < srcSize; ++j) {
            if (!strcmp(src[j], "{")) {
                srcSize = j;
                break;
            }
        }
    } else {
        for (int j = 0; j < srcSize; ++j) {
            if (!strcmp(src[j], ";")) {
                srcSize = j;
                break;
            }
        }
    }

    while(strlen(expr[exprSize].code[0])>0) exprSize++;

    int addBracket = 0;
    int exprInd = 0;
    int i = 0;
    if (srcSize > 1 && strcmp(src[1], "==") != 0 && strcmp(src[1], "=") != 0 && src[1][strlen(src[1]) - 1] == '=') {
        sizeDelta++;
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
        sizeDelta++;

        if(!strcmp(src[0], "if")) boolStack[*bcnt].type = If;
        if(!strcmp(src[0], "while")) boolStack[*bcnt].type = While;
        boolStack[*bcnt].line = metaVal;
        boolStack[*bcnt].fullInit = 1;
        boolStack[*bcnt].isBreak = 0;
        boolStack[*bcnt].cTrendDir = 0;
        boolStack[*bcnt].iTrendDir = 0;
        boolStack[*bcnt].state = 0;

        char **ifCond = (char **) malloc(10 * sizeof(char *));
        for (int y = 0; y < 10; y++) {
            ifCond[y] = (char *) malloc(10 * sizeof(char));
            memset(ifCond[y], 0, 10);
        }
        int ifCondIdx = 0;
        strcpy(ifCond[ifCondIdx], "?");
        strcat(ifCond[ifCondIdx], src[0]);
        char *metaStr = (char *) malloc(5 * sizeof(char));
        sprintf(metaStr, "%d", metaVal);
        strcat(ifCond[ifCondIdx++], metaStr);

        stData tmp = {.str = ifCond[ifCondIdx - 1]};
        stPush(metaData, tmp);

        char **tmpEnd = (char **) malloc(2 * sizeof(char *));
        tmpEnd[0] = (char *) malloc(10 * sizeof(char));
        tmpEnd[1] = (char *) malloc(10 * sizeof(char));
        strcpy(tmpEnd[0], "int");
        strcpy(tmpEnd[1], stTop(metaData).str);

        addExpression(expr, exprSize++, tmpEnd, 2, NULL, 0, NULL, reqSize, NULL, bcnt);

        strcpy(ifCond[ifCondIdx++], "=");
        i = 1;
        for (; strcmp(src[i], "{");) {
            strcpy(ifCond[ifCondIdx++], src[i++]);
        }
        addExpression(expr, exprSize++, ifCond, ifCondIdx, NULL, 0,NULL, reqSize, NULL, bcnt);
        Vec.push(reqSize,0);

        char **tmpBegTag = (char **) malloc(2 * sizeof(char *));
        tmpBegTag[0] = (char *) malloc(10 * sizeof(char));
        tmpBegTag[1] = (char *) malloc(10 * sizeof(char));
        strcpy(tmpBegTag[0], "begin");
        strcpy(tmpBegTag[1], stTop(metaData).str);

        addExpression(expr, exprSize++, tmpBegTag, 2, NULL, 0, NULL, reqSize, NULL, bcnt);
        *(bcnt)+=1;
    } else if (!strcmp(src[0], "for")) {
        sizeDelta++;
        srcSize--;
        boolStack[*bcnt].type = For;
        boolStack[*bcnt].line = metaVal;
        boolStack[*bcnt].fullInit = 1;
        boolStack[*bcnt].isBreak = 0;
        boolStack[*bcnt].cTrendDir = 0;
        boolStack[*bcnt].iTrendDir = 0;
        boolStack[*bcnt].state = 0;

        i = 2;
        char **forInit = (char **) malloc(10 * sizeof(char *));
        int y = 0;
        for (; strcmp(src[i], ";");i++) {
            if(strcmp(src[i], " ") != 0){
                forInit[y] = (char *) malloc(10 * sizeof(char));
                memset(forInit[y], 0, 10);
                strcpy(forInit[y++], src[i]);
            }
        }
        addExpression(expr, exprSize++, forInit, y, NULL, 0, NULL, reqSize, NULL, bcnt);
        boolStack[*bcnt].fullInit = boolStack[*bcnt].fullInit && y;
        sizeDelta++;
        i++;

        char **forCond = (char **) malloc(10 * sizeof(char *));
        for (int y = 0; y < 10; y++) {
            forCond[y] = (char *) malloc(10 * sizeof(char));
            memset(forCond[y], 0, 10);
        }
        int forCondIdx = 0;
        strcpy(forCond[forCondIdx], "?");
        strcat(forCond[forCondIdx], src[0]);
        char *metaStr = (char *) malloc(5 * sizeof(char));
        sprintf(metaStr, "%d", metaVal);
        strcat(forCond[forCondIdx++], metaStr);

        stData tmp = {.str = forCond[forCondIdx - 1]};
        stPush(metaData, tmp);

        char **tmpEnd = (char **) malloc(2 * sizeof(char *));
        tmpEnd[0] = (char *) malloc(10 * sizeof(char));
        tmpEnd[1] = (char *) malloc(10 * sizeof(char));
        strcpy(tmpEnd[0], "int");
        strcpy(tmpEnd[1], stTop(metaData).str);
        boolStack[*bcnt].name = stTop(metaData).str;
        addExpression(expr, exprSize++, tmpEnd, 2, NULL, 0, NULL, reqSize, NULL, bcnt);

        strcpy(forCond[forCondIdx++], "=");
        strcpy(forCond[forCondIdx++], "(");
        for (; strcmp(src[i], ";");i++) {
            if(strcmp(src[i], " ") != 0)strcpy(forCond[forCondIdx++], src[i]);
            if(!strcmp(src[i],">") || !strcmp(src[i],">=")) boolStack[*bcnt].cTrendDir = 1;
            if(!strcmp(src[i],"<") || !strcmp(src[i],"<=")) boolStack[*bcnt].cTrendDir = -1;
        }
        strcpy(forCond[forCondIdx++], ")");
        addExpression(expr, exprSize++, forCond, forCondIdx, NULL, 0,NULL, reqSize, NULL, bcnt);
        boolStack[*bcnt].fullInit = boolStack[*bcnt].fullInit && (4-forCondIdx);

        char **tmpBegTag = (char **) malloc(2 * sizeof(char *));
        tmpBegTag[0] = (char *) malloc(10 * sizeof(char));
        tmpBegTag[1] = (char *) malloc(10 * sizeof(char));
        strcpy(tmpBegTag[0], "begin");
        strcpy(tmpBegTag[1], stTop(metaData).str);

        addExpression(expr, exprSize++, tmpBegTag, 2, NULL, 0, NULL, reqSize, NULL, bcnt);

        sizeDelta++;
        i++;

    } else if (!strcmp(src[0], "}")) {
        //pop exeSt
        int rs = (int) Vec.get(reqSize,reqSize->total-1);
        Vec.delete(reqSize,reqSize->total-1);

        char **forIt = (char **) malloc(10 * sizeof(char *));
        for (int y = 0; y < rs; y++) {
            forIt[y] = (char *) malloc(10 * sizeof(char));
            memset(forIt[y], 0, 10);
            strcpy(forIt[y],(char*)Vec.get(exe,exe->total-y-1));
        }

        for(int y = 0; y <rs; y++){
            Vec.delete(exe, exe->total-1);
        }
        if (rs) addExpression(expr, exprSize++, forIt, rs, NULL, 0, NULL, reqSize, NULL, bcnt);

        char **tmpEnd = (char **) malloc(2 * sizeof(char *));
        tmpEnd[0] = (char *) malloc(10 * sizeof(char));
        tmpEnd[1] = (char *) malloc(10 * sizeof(char));
        strcpy(tmpEnd[0], "endof");
        strcpy(tmpEnd[1], stTop(metaData).str);

        addExpression(expr, exprSize++, tmpEnd, 2, NULL, 0, NULL, reqSize, NULL, bcnt);

        stPop(metaData);
        i = 1;
    }

    if (!strcmp(src[0], "for")) {
        //push to exeSt
        char **forIt = (char **) malloc(10 * sizeof(char *));
        for (int y = 0; y < 10; y++) {
            forIt[y] = (char *) malloc(10 * sizeof(char));
            memset(forIt[y], 0, 10);
        }
        int y = 0;
        int j = i;
        for (; srcSize-i>0; ++i) {
            if(strcmp(src[i]," ")){
                y++;
                Vec.push(exe,src[srcSize-y]);
            }
        }
        Vec.push(reqSize,y);
        if (y) {
            boolStack[*bcnt].iter = (char *) malloc(10 * sizeof(char));
            strcpy(boolStack[*bcnt].iter, src[j]);
            if(!strcmp(src[j+1],"+=") || !strcmp(src[j+1],"++")) boolStack[*bcnt].iTrendDir = 1;
            if(!strcmp(src[j+1],"-=") || !strcmp(src[j+1],"--")) boolStack[*bcnt].iTrendDir = -1;
        }
        boolStack[*bcnt].fullInit = boolStack[*bcnt].fullInit && y;
        *(bcnt)+=1;
    } else {
        for (; i < srcSize; ++i) {
            strcpy(expr[exprSize].code[exprInd++], src[i]);
        }
    }

    if (addBracket) {
        strcpy(expr[exprSize].code[exprInd++], ")");
    }

    expr[exprSize].size = exprInd;

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