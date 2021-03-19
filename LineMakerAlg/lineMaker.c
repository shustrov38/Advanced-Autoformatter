#include "lineMaker.h"

codeLineStruct *init(){
    codeLineStruct *codeBody = malloc(sizeof(codeLineStruct));
    codeBody->codeLines = (char***)malloc(SIZE*sizeof(char**));
    for (int i = 0; i < SIZE; ++i){
        codeBody->codeLines[i] = (char**)malloc(SIZE*sizeof(char*));
        for (int j = 0; j < ONE_STRING_SIZE; ++j){
            codeBody->codeLines[i][j] = (char*)malloc(ONE_STRING_SIZE*sizeof(char));
        }
    }
    codeBody->linesCnt = 0;
    return codeBody;
}


codeLineStruct *splitStringAlgorithm(codeLineStruct *codeBody, int len, char **originString) {
    int codeLineCnt = 0; int codeWordsCnt = 0;

    int bracketsFlag = 0; int structFlag = 0;

    for (int i = 0; i < len; ++i) {
        if (isSemicolon(originString[i]) && !bracketsFlag){
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            ++i;
        }

        if (isOpenFigBr(originString[i])){
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            ++i;
        }

        if (isCloseFigBr(originString[i])){
            if (structFlag){
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                ++i;
                codeWordsCnt++;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
                codeLineCnt++;
                codeWordsCnt = 0;
                --structFlag;
            } else {
                codeWordsCnt = 0;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
                codeLineCnt++;
                codeWordsCnt = 0;
            }
            ++i;
        }

        if (isStruct(originString[i])){
            structFlag = 1;
        }

        if (isOpenBr(originString[i])) ++bracketsFlag;

        if (isCloseBr(originString[i])) --bracketsFlag;

        strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
        codeWordsCnt++;
    }


    for (int j = 0; j < codeLineCnt; ++j) {
        int i = 0;
        while (strcmp(codeBody->codeLines[j][i], "\0") != 0) {
            printf("%s ", codeBody->codeLines[j][i]);
            ++i;
        }
        printf("\n");
    }

    return 0;
}