#include "outputFormat.h"
#include "lineMaker.h"
#include "../fileUtils.h"
#include "../functions.h"

void outputFiles(char *fileName, codeLineStruct *code) {


    for (int i = 0; i < code->linesCnt; ++i) {
        int len = getLineLength(code->codeLines[i]);
        char outputString[len * 2][30];
        int cycleFlag = 0;
        int nesting = 0;
        int k = 0, j = 0;

        for (; k < nesting; ++k){
            strcpy(outputString[k], "\t");
        }

        //0 Symbol
        strcpy(outputString[k], code->codeLines[i][0]);
        k++;
        strcpy(outputString[k], " ");
        k++;
        if (isBlock(code->codeLines[i][0])) {
            cycleFlag++;
        }
//        if (isCloseFigBr(code->codeLines[i][0])){
//            nesting--;
//        }

        //one string check

        for (int s = k+1; s < len; ++s){
            //printf("%s ", code->codeLines[i][s]);

            if (!strcmp(code->codeLines[i][s], " ")){
                continue;
            }
            if (isOpenBr(code->codeLines[i][s])) {
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
                //TODO: func & unary
            }

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

            if (isOpenFigBr(code->codeLines[i][s])){
                nesting++;
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
            }

            if (isSemicolon(code->codeLines[i][s])){
                if (!strcmp(outputString[k], " ")){
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                } else {
                    strcpy(outputString[k+1], code->codeLines[i][s]);
                    k += 2;
                    strcpy(outputString[k], " ");
                    k++;
                }
                continue;
            }
            strcpy(outputString[k], code->codeLines[i][s]);
            k++;
            strcpy(outputString[k], " ");
            k++;
        }
        for (int c = 0; c < k; ++c){
            printf("%s", outputString[c]);
        } printf("\n");

    }
}