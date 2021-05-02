#include "parser.h"

const char *divs[DIVISION_DIVERSITY] = {
        "//", ">>=", "<<=", "/*", "*/", "\'",
        "+=", "-=", "*=", "/=", "==", "++", "--", ">=", "<=",
        "!=", "&&", "||", "^^", "^=", "|=", "&=", "~=", ">>", "<<",
        ">", "<", "+", "-", "*", "/", "=",
        "!", "?", "&", "|", "^", "~",
        "(", ")", "[", "]", "{", "}",
        " ", ".", ",", ";", ":", "\n", "\t", "\""
};

// TODO: add #define, #ifdef, #ifndef, #endif, #undef

int splitSyntax(const char *srcFile, char **dest) {
    FILE *in = fopen(srcFile, "r");

    if (in == NULL) {
        printf("File \"%s\" doesnt exists.\n", srcFile);
        exit(-2);
    }

    int destBlockIndex = 0;
    int destInBlockCharIndex = 0;

    fseek(in, 0, SEEK_END);
    int fLastChar = ftell(in);
    fseek(in, 0, SEEK_SET);

    char *src = malloc(fLastChar * sizeof(char));
    memset(src, 0, fLastChar);
    fread(src, 1, fLastChar, in);

    fclose(in);

    int srcCharIndex = 0;
    int separatorFlag = 1; //is op flag for ch

    //quotation logics
    int strQF = 0;
    int chrQF = 0;
    int strCF = 0;
    int blkCF = 0;
    //***

    while (srcCharIndex < fLastChar) {
        int separatorId = -1;
        int divisorMeetFlag = 0;
        for (int separatorListIndex = 0; separatorListIndex < DIVISION_DIVERSITY; separatorListIndex++) {
            char stringApplicator[MAX_DIVISOR_LEN];
            memset(stringApplicator, 0, MAX_DIVISOR_LEN);
            for (int applicatorIndex = 0; applicatorIndex < strlen(divs[separatorListIndex]); applicatorIndex++) {
                stringApplicator[applicatorIndex] = src[srcCharIndex + applicatorIndex];
            }
            stringApplicator[strlen(divs[separatorListIndex])] = 0;
            if (!strcmp(stringApplicator, divs[separatorListIndex])) {
                divisorMeetFlag = 1;
                separatorId = separatorListIndex;
                break;
            }
        }

        int qF = strQF + chrQF + strCF + blkCF;
        //update quotation logics
        if (separatorId > -1) {
            if (!strcmp(divs[separatorId], "'")) {
                chrQF = (chrQF ^ 1) & 1;
            }
            if (!strcmp(divs[separatorId], "\"")) {
                strQF = (strQF ^ 1) & 1;
            }
            if (!strcmp(divs[separatorId], "//")) strCF = 1;
            if (!strcmp(divs[separatorId], "\n")) strCF = 0;
            if (!strcmp(divs[separatorId], "/*")) blkCF = 1;
            if (!strcmp(divs[separatorId], "*/")) blkCF = 0;
        }
        //***
        if (!divisorMeetFlag) {
            dest[destBlockIndex][destInBlockCharIndex++] = src[srcCharIndex++];
            separatorFlag = 0;
        } else {
            destInBlockCharIndex = 0;
            if (!separatorFlag) destBlockIndex++;

            if (qF ||
                !(!strcmp(divs[separatorId], " ") ||
                  !strcmp(divs[separatorId], "\t") ||
                  !strcmp(divs[separatorId], "\n") ||
                  !strcmp(divs[separatorId], "\0"))) { //list of SKIPPABLE DIVISORS
                strcpy(dest[destBlockIndex], divs[separatorId]);

                ++destBlockIndex;
            }
            srcCharIndex += (int) strlen(divs[separatorId]);
            separatorFlag = 1;
        }

    }

    free(src);

    return destBlockIndex + 1; /* -> return := block cnt */
}