//q
//TODO: rework into pointers+no le majique numbers; add debug out;
#include "parser.h"

#define MAX_ARRAY_SIZE 10000

int splitSyntax(char *srcFile, char **dest, char *divs[56]) {

    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }

    int destBlockIndex = 0;
    char *src = malloc(MAX_ARRAY_SIZE * sizeof(char));
    while (fgets(src, MAX_ARRAY_SIZE, in)) {
        int srcCharIndex = 0;
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
                dest[destBlockIndex][destInBlockCharIndex++] = src[srcCharIndex++];
                separatorFlag = 0;
            } else {
                destInBlockCharIndex = 0;
                if (!separatorFlag) destBlockIndex++;

                if (!(!strcmp(divs[separatorId], " ") || !strcmp(divs[separatorId], "\t"))) {
                    strcpy(dest[destBlockIndex], divs[separatorId]);

                    ++destBlockIndex;
                }
                srcCharIndex += (int) strlen(divs[separatorId]);
                separatorFlag = 1;
            }
        }
    }

    free(src);

    return destBlockIndex; // -> return := block cnt
}