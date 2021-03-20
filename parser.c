#include "parser.h"

#define MAX_ARRAY_SIZE 1000

int splitSyntax(char *srcFile, char dest[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE], char *divs[56]) {

    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }

    char *src = malloc(MAX_ARRAY_SIZE * sizeof(char));
    while (fgets(src, MAX_ARRAY_SIZE, in)) {
        fprintf(stdout, "\n%s\n", src);
        char tmpStr[MAX_ARRAY_SIZE][MAX_ARRAY_SIZE];
        for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
            memset(tmpStr[i], 0, MAX_ARRAY_SIZE);
        }
        int srcCharIndex = 0;
        int destBlockIndex = 0;
        int destInBlockCharIndex = 0;
        int separatorId = -1;
        int separatorFlag = 1; //is op flag for ch

        while (src[srcCharIndex] != '\n' && src[srcCharIndex] != '\0' && src[srcCharIndex] != '\r') {
            int divisorMeetFlag = 0;
            for (int separatorListIndex = 0; separatorListIndex < 56; separatorListIndex++) {
                char stringApplicator[100];
                memset(stringApplicator, 0, 100);
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
            if (!divisorMeetFlag) {
                tmpStr[destBlockIndex][destInBlockCharIndex++] = src[srcCharIndex++];
                separatorFlag = 0;
            } else {
                destInBlockCharIndex = 0;
                if (!separatorFlag) destBlockIndex++;

                strcpy(tmpStr[destBlockIndex], divs[separatorId]);

                ++destBlockIndex;
                srcCharIndex += (int) strlen(divs[separatorId]);
                separatorFlag = 1;
            }
        }

        int transferingIndex = 0;
        for (srcCharIndex = 0; srcCharIndex < MAX_ARRAY_SIZE; ++srcCharIndex) {
            if ((int) strlen(tmpStr[srcCharIndex]) > 0) strcpy(dest[transferingIndex++], tmpStr[srcCharIndex]);
        }
        for (srcCharIndex = 0; srcCharIndex < transferingIndex; srcCharIndex++) {
            fprintf(stdout, "%s ", dest[srcCharIndex]);
        }
    }

    return 0;
}