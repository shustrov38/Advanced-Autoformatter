#include "outputFormat.h"
#include "lineMaker.h"
#include "../fileUtils.h"
#include "../functions.h"
#include "../Calculator/ops.h"

void outputFiles(char *fileName, codeLineStruct *code) {
    FILE *file;
    file = fopen(fileName, "w");

    //TODO
    // "
    // comments spaces
    // ++a
    // "

    int nesting = 0;
    int caseFlag = 0;
    int switchFlag = 0;
    int switchCnt = 0;
    int includeFlag = 0;
    for (int i = 0; i < code->linesCnt; ++i) {
        int len = getLineLength(code->codeLines[i]);

        OpInfo *info = getLineOfIDs(code->codeLines[i], len);

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
            strcpy(outputString[k], "   ");
        }


        if(!strcmp(code->codeLines[i][0], "switch")) {
            nesting++;
            switchFlag=1;
            switchCnt++;
        }

        //0 Symbol
        if (info[0].id == PREF_INC || info[0].id == PREF_DEC) {
            strcpy(outputString[0], code->codeLines[i][0]);
            ++k;

        } else {
            strcpy(outputString[k], code->codeLines[i][0]);
            k++;
            strcpy(outputString[k], " ");
            k++;
        }

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
                if (info[s-1].id == VAR && !isBlock(code->codeLines[i][s-1])){
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                    continue;
                } else {
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                    continue;
                }
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

            if (!strcmp(code->codeLines[i][s], ",")){
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
            //open #include
            if (!strcmp(code->codeLines[i][s], "<") && !strcmp(code->codeLines[i][s-1], "#include")){
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                includeFlag++;
                continue;
            }
            //close #include
            if (!strcmp(code->codeLines[i][s], ">") && includeFlag > 0){
                strcpy(outputString[k], code->codeLines[i][s]);
                includeFlag--;
                k++;
                continue;
            }

            //scanf &
            if (!strcmp(code->codeLines[i][s], "&")){
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
            }


            //Open fig br
            if(isOpenFigBr(code->codeLines[i][s]) && isCloseFigBr(code->codeLines[i][s+2])) {

            }
            else if (isOpenFigBr(code->codeLines[i][s])){
                nesting++;
                strcpy(outputString[k], code->codeLines[i][s]);
                k++;
                continue;
            }

            //square brackets
            if (!strcmp(code->codeLines[i][s], "[")){
                if (!strcmp(outputString[k-1], " ")){
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                } else {
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                }
                continue;
            }

            if (!strcmp(code->codeLines[i][s], "]")){
                if (!strcmp(outputString[k-1], " ")){
                    strcpy(outputString[k-1], code->codeLines[i][s]);
                    strcpy(outputString[k], " ");
                    k++;
                } else {
                    strcpy(outputString[k], code->codeLines[i][s]);
                    k++;
                }
                continue;
            }

            //unary
            if (info[s].id == POST_INC || info[s].id == POST_DEC){
                strcpy(outputString[k-1], code->codeLines[i][s]);
                strcpy(outputString[k], " ");
                ++k;
                continue;
            } else if (info[s].id == PREF_INC || info[s].id == PREF_DEC) {
                strcpy(outputString[k], code->codeLines[i][s]);
                ++k;
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
            if (includeFlag == 0) {  //use spaces if it's not #include
                strcpy(outputString[k], " ");
                k++;
            }
        }

        //output
        for (int c = 0; c < k; ++c){
            fprintf(file, "%s", outputString[c]);
            strcpy(outputString[c], "\0");
        } fprintf(file, "\n");

    }
}