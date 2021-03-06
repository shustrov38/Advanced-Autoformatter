#include "lineMaker.h"
#include "optionFunctions.h"

#define ONE_FILE_SIZE 200
#define ONE_LINE_SIZE 50
#define ONE_ELEMENT_SIZE 30

codeLineStruct *createCodeLineStruct() {
    codeLineStruct *codeBody = malloc(sizeof(codeLineStruct));
    codeBody->codeLines = (char ***) malloc(ONE_FILE_SIZE * sizeof(char **));
    for (int i = 0; i < ONE_FILE_SIZE; ++i) {
        codeBody->codeLines[i] = (char **) malloc(ONE_LINE_SIZE * sizeof(char *));
        for (int j = 0; j < ONE_LINE_SIZE; ++j) {
            codeBody->codeLines[i][j] = (char *) malloc(ONE_ELEMENT_SIZE * sizeof(char));
            memset(codeBody->codeLines[i][j], 0, ONE_ELEMENT_SIZE);
        }
    }
    codeBody->linesCnt = 0;

    return codeBody;
}

void printCode(codeLineStruct *code) {
    for (int j = 0; j < code->linesCnt; ++j) {
        for (int k = 0; code->codeLines[j][k] != NULL && code->codeLines[j][k][0]; ++k) {
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

    char commentLine[100][100];
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            commentLine[i][j] = 0;
        }
    }

    //TODO: struct [ }; ], [ ") ]
    for (int i = 0; i < len; ++i) {
        if (isSemicolon(originString[i]) && !bracketsFlag) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            i++;

            if (isCloseFigBr(originString[i])) {
                strcpy(codeBody->codeLines[codeLineCnt][0], originString[i]);
                codeLineCnt++;
                ++i;
                --figBracketCnt;
            }

            if (doFlag) {
                ++codeWordsCnt;
                --codeLineCnt;
                while (strcmp(originString[i], ";")) {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    ++i;
                    codeWordsCnt++;
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                ++i;
                codeLineCnt++;
                codeWordsCnt = 0;
                doFlag--;
            }

            while (figBracketCnt > 0) {
                strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                codeLineCnt++;
                --figBracketCnt;
            }
        }

//        if(isOpenFigBr(originString[i]) && isCloseFigBr(originString[i+2])) {
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            i++;
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            i++;
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            continue;
//        }

        if(isOpenFigBr(originString[i]) && (!strcmp(originString[i-1], "="))) {
            while(!isCloseFigBr(originString[i])) {
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                i++;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            continue;
        }

        if (isOpenFigBr(originString[i])) {
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

        if (!strcmp(originString[i], ":")) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            ++i;
            codeLineCnt++;
            codeWordsCnt = 0;
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
            int k = 1;
            int isInsideComment = 0;
            int afterForCommentFlag = 0;
            int commentFlag = 0;
            if (isSlashAndStar(originString[i])) {
                afterForCommentFlag = 1;
                isInsideComment = 1;
                strcpy(commentLine[0], "//");
                ++i;

                while (strcmp(originString[i], "*/")) {
                    strcpy(commentLine[k], originString[i]);
                    ++k;
                    ++i;
                }
                isInsideComment = 0;
                ++i;
                while(strcmp(originString[i], "(")) {
                    if(isSlashAndStar(originString[i])){
                        ++i;
                        isInsideComment = 1;
                        continue;
                    }
                    if(!strcmp(originString[i], "*/")) {
                        ++i;
                        isInsideComment = 0;
                        continue;
                    }
                    strcpy(commentLine[k], originString[i]);
                    ++k;
                    ++i;
                }
                commentFlag = i;
                strcpy(commentLine[k], originString[i]);
                ++k;
                ++i;
                int brackets = 1;
                while (brackets) {
                    if (isOpenBr(originString[i])) {
                        ++brackets;
                    } else if (isCloseBr(originString[i])) {
                        --brackets;
                    }
                    if(isSlashAndStar(originString[i])) {
                        isInsideComment = 1;
                        ++i;
                        continue;
                    }
                    else if(!strcmp(originString[i], "*/")) {
                        isInsideComment = 0;
                        ++i;
                        continue;
                    }

                    strcpy(commentLine[k], originString[i]);
                    ++k;
                    ++i;
                }
                i = commentFlag;
            }

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

                    if (isSlashAndStar(originString[i])) {
                        commentFlag = 1;
                        strcpy(commentLine[0], "//");
//                        ++k;
                        ++i;
                        continue;
                    }
                    if (!strcmp(originString[i], "*/")) {
                        commentFlag = 2;
                        ++i;
                        continue;
                    }
                    if (commentFlag == 1) {
                        if(!afterForCommentFlag) {
                            strcpy(commentLine[k], originString[i]);
                        }
                        ++k;
                        ++i;
                    } else if (commentFlag == 2) {
                        strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                        if(!afterForCommentFlag) {
                            strcpy(commentLine[k], originString[i]);
                        }
                        ++k;
                        codeWordsCnt++;
                        i++;
                    } else {
                        strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                        codeWordsCnt++;
                        i++;
                    }
                }
            }


            if (!isOpenFigBr(originString[i])) {
                figBracketCnt++;
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "{");
                codeWordsCnt++;
                for (int j = 0; j < k; ++j) {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], commentLine[j]);
                    ++codeWordsCnt;
                    strcpy(commentLine[j], "\0");
                }
                codeLineCnt++;
                codeWordsCnt = 0;
                --i;
                continue;
            }

            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            for (int j = 0; j < k; ++j) {
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], commentLine[j]);
                ++codeWordsCnt;
                strcpy(commentLine[j], "\0");
            }
            codeLineCnt++;
            codeWordsCnt = 0;
            continue;
        }

        if (!strcmp(originString[i], "#define")) {
            int quotationCnt = 0;
            int commentFlag = 0;
            int k = 0;
            int defineWords = 0;
            while (defineWords < 3) {
                if (isSlashAndStar(originString[i])) {
                    strcpy(commentLine[k], "//");
                    commentFlag = 1;
                    ++i;
                    ++k;
                }
                if (!strcmp(originString[i], "*/")) {
                    ++i;
                    commentFlag = 2;
                }

                if (commentFlag == 1) {
                    strcpy(commentLine[k], originString[i]);
                    ++k;
                    ++i;
                } else if (commentFlag == 2) {
                    strcpy(commentLine[k], originString[i]);
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    codeWordsCnt++;
                    ++k;
                    ++i;
                    defineWords++;
                } else {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    codeWordsCnt++;
                    ++i;
                    ++defineWords;
                }
            }
            for (int j = 0; j < k; ++j) {
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], commentLine[j]);
                strcpy(commentLine[j], "\0");
                codeWordsCnt++;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeWordsCnt = 0;
            codeLineCnt++;
            i--;
            continue;
        }

//        if (!strcmp(originString[i], "#endif")){
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
//            codeWordsCnt = 0;
//            codeLineCnt++;
//            continue;
//        }
//
//        if (!strcmp(originString[i], "#ifdef") || !strcmp(originString[i], "#ifndef")){
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            i++;
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
//            codeWordsCnt++;
//            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
//            codeWordsCnt = 0;
//            codeLineCnt++;
//            continue;
//        }

        if (!strcmp(originString[i], "#include")) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            ++i;
            int j = 1;
            int commentFlag = 0;
            if (isSlashAndStar(originString[i])) {
                commentFlag = 1;
                strcpy(commentLine[0], "//");
                ++i;
                while (strcmp(originString[i], "*/")) {
                    strcpy(commentLine[j], originString[i]);
                    ++i;
                    ++j;
                }
                ++i;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            if (commentFlag) {
                strcpy(commentLine[j], originString[i]);
                ++j;
            }
            codeWordsCnt++;
            ++i;
            while (strcmp(originString[i], "\"") && strcmp(originString[i], ">")) {
                if (commentFlag) {
                    strcpy(commentLine[j], originString[i]);
                    ++j;
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                ++i;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            codeWordsCnt++;
            if (commentFlag) {
                strcpy(commentLine[j], originString[i]);
                ++j;
            }
            if (commentFlag) {
                for (int m = 0; m < j; ++m) {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], commentLine[m]);
                    strcpy(commentLine[j], "\0");
                    codeWordsCnt++;
                }
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeWordsCnt = 0;
            codeLineCnt++;
            continue;
        }

        if (isDoubleSlash(originString[i])) {
            while (strcmp(originString[i], "\n") && i < len) {
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                codeWordsCnt++;
                ++i;
            }
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "\0");
            codeLineCnt++;
            codeWordsCnt = 0;
            continue;
        }

        if (isSlashAndStar(originString[i]) && codeWordsCnt == 0) {
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "//");
            codeWordsCnt++;
            ++i;
            while (strcmp(originString[i], "*/")) {
                if (!strcmp(originString[i], "\n") && strcmp(originString[i + 1], "*/")) {
                    codeLineCnt++;
                    codeWordsCnt = 0;
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], "//");
                    codeWordsCnt++;
                    ++i;
                } else {
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    codeWordsCnt++;
                    ++i;
                }
            }
            codeWordsCnt = 0;
            codeLineCnt++;
            continue;
        } else if (isSlashAndStar(originString[i]) && codeWordsCnt != 0) {
            ++i;
            int j = 1;
            int inComment = 1;
            strcpy(commentLine[0], "//");
            while (!((!strcmp(originString[i], ";") || !strcmp(originString[i], "{")) && !inComment)) {
                if(!strcmp(originString[i], "*/")) {
                    inComment = 0;
                    ++i;
                    continue;
                }
                if(!strcmp(originString[i], "/*")) {
                    inComment = 1;
                    ++i;
                    continue;
                }
                if(!strcmp(originString[i], "\n")) {
                    ++i;
                    continue;
                }
                if(inComment) {
                    strcpy(commentLine[j], originString[i]);
                    ++i;
                    ++j;
                }
                else {
                    strcpy(commentLine[j], originString[i]);
                    strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
                    ++i;
                    ++j;
                    codeWordsCnt++;
                }
            }
            strcpy(commentLine[j], originString[i]);
            strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], originString[i]);
            ++i;
            ++j;
            codeWordsCnt++;
            int spaceCnt = 0;
            for (int k = 0; k < j; ++k) {
                if(!strcmp(commentLine[k], " ")) {
                    spaceCnt++;
                    if(spaceCnt > 0) {
                        continue;
                    }
                }
                strcpy(codeBody->codeLines[codeLineCnt][codeWordsCnt], commentLine[k]);
                strcpy(commentLine[k], "\0");
                codeWordsCnt++;
                spaceCnt = 0;
            }
            codeWordsCnt = 0;
            codeLineCnt++;
            --i;
            while (figBracketCnt) {
                strcpy(codeBody->codeLines[codeLineCnt][0], "}");
                codeLineCnt++;
                --figBracketCnt;
            }
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



