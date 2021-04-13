#include "outputFormat.h"
#include "lineMaker.h"
#include "../fileUtils.h"
#include "../functions.h"

void outputFiles(char *fileName, codeLineStruct *code) {
    //TODO: change do while in lineMaker

    int nesting = 0;
    int caseFlag=0;
    int switchFlag=0;
    int switchCnt=0;
    for (int i = 0; i < code->linesCnt; ++i) {
        int len = getLineLength(code->codeLines[i]);
        char outputString[len * 3][60];
        int cycleFlag = 0;

        int k = 0, j = 0;


        if (isCloseFigBr(code->codeLines[i][0])) {
            if (switchFlag) {
                nesting -= 2;
                switchFlag = 0;
            }
            else {
                --nesting;
            }
            caseFlag = 0;
        }


        if(!strcmp(code->codeLines[i][0], "case")) {
            caseFlag=1;
            nesting--;
        }

        for (k = 0; k < nesting; ++k) {
            strcpy(outputString[k], "    ");
        }

        if(!strcmp(code->codeLines[i][0], "switch")) {
            nesting++;
            switchFlag=1;
            switchCnt++;
        }

        //0 Symbol
        strcpy(outputString[k], code->codeLines[i][0]);
        k++;
        strcpy(outputString[k], " ");
        k++;

        if (isBlock(code->codeLines[i][0])) {
            cycleFlag++;
        }
        if (caseFlag) {
            nesting++;
            caseFlag=0;
        }


        //one string check
        for (int s = 1; s < len; ++s){

            //skip space
            if (!strcmp(code->codeLines[i][s], " ")){
                continue;
            }

            //open br
            if (isOpenBr(code->codeLines[i][s])) {
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
                //TODO: func & unary
            }

            //close br
            if (isCloseBr(code->codeLines[i][s])){
                if (isOpenBr(outputString[k-1])){
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                } else {
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                    strcpy(outputString[k], " ");
                    k++;
                }
                continue;
            }

            //Open fig br
            if (isOpenFigBr(code->codeLines[i][s])){
                nesting++;
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
            }

            //square brackets
            if (!strcmp(code->codeLines[i][s], "[") || !strcmp(code->codeLines[i][s], "]")){
                if (!strcmp(outputString[k-1], " ")){
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                } else {
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                }
                continue;
            }

            //semicolon and two dots symbol
            if (isSemicolon(code->codeLines[i][s]) || !strcmp(code->codeLines[i][s], ":")){
                if (!strcmp(outputString[k-1], " ")){
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                    strcpy(outputString[k], " ");
                    k++;
                } else {
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                    strcpy(outputString[k], " ");
                    k++;
                }
                continue;
            }

            //simple Symbol
           strcpy(outputString[k], code->codeLines[i][s]);
            k++;
            strcpy(outputString[k], " ");
            k++;
        }

        //output
        for (int c = 0; c < k; ++c){
            printf("%s", outputString[c]);
        } printf("\n");

    }
}