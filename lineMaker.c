#include "lineMaker.h"
#include "optionFunctions.h"

#define SIZE 10000
#define ONE_STRING_SIZE 100

codeLineStruct *createCodeLineStruct() {
    codeLineStruct *codeBody = malloc(sizeof(codeLineStruct));
    codeBody->codeLines = (char ***) malloc(SIZE * sizeof(char **));
    for (int i = 0; i < SIZE; ++i) {
        codeBody->codeLines[i] = (char **) malloc(SIZE * sizeof(char *));
        for (int j = 0; j < ONE_STRING_SIZE; ++j) {
            codeBody->codeLines[i][j] = (char *) malloc(ONE_STRING_SIZE * sizeof(char));
        }
    }
    codeBody->linesCnt = 0;

    return codeBody;
}

void printCode(codeLineStruct *code) {
    for (int j = 0; j < code->linesCnt; ++j) {
        for (int k = 0; code->codeLines[j][k]; ++k) {
            printf("%s ", code->codeLines[j][k]);
        }
        printf("\n");
    }
}

void splitLines(codeLineStruct *codeBody, int len, char **originString) {
    int codeLineCnt = 0;
    int codeWordsCnt = 0;

    int bracketsFlag = 0;
    int structFlag = 0;
    int figBracketCnt = 0;
    int doFlag = 0;

    for (int i = 0; i < len; ++i) {
        if (isSemicolon(originString[i]) && !bracketsFlag) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            ++i;

            if (isCloseFigBr(originString[i])) {
                strcpy(codeBody->codeLines[codeLineCnt][0], originString[i]);
                codeLineCnt++;
                ++i;
            }

            if (doFlag) {
                if (!isCloseFigBr(codeBody->codeLines[codeLineCnt - 1][0])) {
                    strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                    codeLineCnt++;
                    --figBracketCnt;
                }
                codeWordsCnt = 0;
                while (!isSemicolon(originString[i])) {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    ++i;
                    codeWordsCnt++;
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeLineCnt++;
                codeWordsCnt = 0;
            }

            while (figBracketCnt) {
                strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                codeLineCnt++;
                --figBracketCnt;
            }
        }

        if (isOpenFigBr(originString[i])) {
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            continue;
        }

        if (isCloseFigBr(originString[i])) {
            if (structFlag) {
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
                if (isSemicolon(originString[i + 1])) {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i + 1]);
                    codeLineCnt++;
                    codeWordsCnt = 0;
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
                codeLineCnt++;
                codeWordsCnt = 0;
            }
            continue;
        }

        if (isStruct(originString[i])) {
            structFlag = 1;
        }

        if (isOpenBr(originString[i])) ++bracketsFlag;

        if (isCloseBr(originString[i])) --bracketsFlag;

        if (isBlock(originString[i])) {
            if (isDo(originString[i])) {
                doFlag = 1;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            ++i;
            int brackets = 0;
            if (isOpenBr(originString[i])) {
                ++brackets;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                ++i;
                while (brackets) {
                    if (isOpenBr(originString[i])) {
                        ++brackets;
                    } else if (isCloseBr(originString[i])) {
                        --brackets;
                    }
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    codeWordsCnt++;
                    i++;
                }
            }

            if (!isOpenFigBr(originString[i])) {
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

        if (!strcmp(originString[i], "#define")) {
            strcpy(codeBody->codeLines[codeLineCnt][0], originString[i]);
            ++i;
            strcpy(codeBody->codeLines[codeLineCnt][1], originString[i]);
            ++i;
            strcpy(codeBody->codeLines[codeLineCnt][2], originString[i]);
            ++i;
            strcpy(codeBody->codeLines[codeLineCnt][3], "\0");
            codeLineCnt++;
        }

        if (!strcmp(originString[i], "#include")) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            ++i;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            ++i;
            while (strcmp(originString[i], "\"") && strcmp(originString[i], ">")) {
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                ++i;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeWordsCnt++;
            codeLineCnt++;
            continue;
        }

        strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
        codeWordsCnt++;
    }

    codeBody->linesCnt = codeLineCnt;

#ifdef __LINE_MAKER_DEBUG__
    printCode(codeBody);
#endif
}



