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


//void checkCycleBody(codeLineStruct *codeBody, int *curLine, char **originString, int *curPos){
//    int bodyWordsCnt = 0;
//    strcpy(codeBody->codeLines[*curLine][bodyWordsCnt], originString[*curPos]);
//    *curPos++; bodyWordsCnt++;
//    if (isOpenBr(originString[*curPos])){
//        int bracketCnt = 1;
//        strcpy(codeBody->codeLines[*curLine][bodyWordsCnt], originString[*curPos]);
//        *curPos++; bodyWordsCnt++;
//        while(bracketCnt){
//            if (isOpenBr(originString[*curPos])) {
//                bracketCnt++;
//            } else if(isCloseBr(originString[*curPos])) {
//                bracketCnt--;
//            }
//            strcpy(codeBody->codeLines[*curLine][bodyWordsCnt], originString[*curPos]);
//            *curPos++; bodyWordsCnt++;
//        }
//    }
//
//    if (isOpenFigBr(originString[*curPos])){
//        return;
//    } else {
//        *curLine++;
//        strcpy(codeBody->codeLines[*curLine][0], "{");
//
//        *curLine++;
//        if (isCycle(originString[*curPos])){
//            checkCycleBody(codeBody, &curLine, originString, &curPos);
//        }
//
//
//
//    }
//}

codeLineStruct *splitStringAlgorithm(codeLineStruct *codeBody, int len, char **originString) {
    int codeLineCnt = 0; int codeWordsCnt = 0;

    int bracketsFlag = 0; int structFlag = 0; int figBracketCnt = 0; int doFlag = 0;

    for (int i = 0; i < len; ++i) {
        if (isSemicolon(originString[i]) && !bracketsFlag){
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            ++i;

            if (isCloseFigBr(originString[i])){
                strcpy(codeBody->codeLines[codeLineCnt][0], originString[i]);
                codeLineCnt++;
                ++i;
            }

            if (doFlag){
                if (!isCloseFigBr(codeBody->codeLines[codeLineCnt-1][0])){
                    strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                    codeLineCnt++;
                    --figBracketCnt;
                }
                codeWordsCnt = 0;
                while(!isSemicolon(originString[i])){
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    ++i; codeWordsCnt++;
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeLineCnt++;
                codeWordsCnt = 0;
            }

            while(figBracketCnt){
                strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                codeLineCnt++;
                --figBracketCnt;
            }
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

        if (isCycle(originString[i])){
            if(isDo(originString[i])){
                doFlag = 1;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            ++i;
            int brackets = 0;
            if(isOpenBr(originString[i])){
                ++brackets;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                ++i;
                while (brackets){
                    if (isOpenBr(originString[i])) {
                        ++brackets;
                    } else if (isCloseBr(originString[i])) {
                        --brackets;
                    }
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    codeWordsCnt++; i++;
                }
            }

            if (!isOpenFigBr(originString[i])){
                figBracketCnt++;
                codeLineCnt++;
                codeWordsCnt = 0;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "{");
                codeLineCnt++;
                --i;
                continue;
            }
            codeLineCnt++;
            codeWordsCnt = 0;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeLineCnt++;
            continue;
        }

        strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
        codeWordsCnt++;
    }

    //debug
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



