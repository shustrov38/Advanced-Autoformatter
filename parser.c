//TODO: rework into pointers+no le majique numbers; add debug out;
#include "parser.h"

int splitSyntax(char *srcFile, char **dest, const char *divs[51]) {
    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }

    int destBlockIndex = 0;
    char *src = malloc(MAX_CODE_LEN * sizeof(char));
    memset(src, 0, MAX_CODE_LEN);
    while (fgets(src, MAX_CODE_LEN, in)) {
        int srcCharIndex = 0;
        int destInBlockCharIndex = 0;
        int separatorId = -1;
        int separatorFlag = 1; //is op flag for ch

        while (src[srcCharIndex] != '\n' && src[srcCharIndex] != '\0' && src[srcCharIndex] != '\r') {
            int divisorMeetFlag = 0;
            for (int separatorListIndex = 0; separatorListIndex < 51; separatorListIndex++) {
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
            if (!divisorMeetFlag) {
                dest[destBlockIndex][destInBlockCharIndex++] = src[srcCharIndex++];
                separatorFlag = 0;
            } else {
                destInBlockCharIndex = 0;
                if (!separatorFlag) destBlockIndex++;

//                if (!(!strcmp(divs[separatorId], " ") || !strcmp(divs[separatorId], "\t") || !strcmp(divs[separatorId], "\n"))) { //list of SKIPPABLE DIVISORS
                strcpy(dest[destBlockIndex], divs[separatorId]);

                ++destBlockIndex;
//                }
                srcCharIndex += (int) strlen(divs[separatorId]);
                separatorFlag = 1;
            }
        }
    }

    free(src);
    fclose(in);
    return
            destBlockIndex; /* -> return := block cnt */
}