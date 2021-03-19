#include "parser.h"

#define MAX_ARRAY_SIZE 10000
#define MAX_E_SIZE 10000

void initSyntax(codeSyntax *syntax) {
    syntax->codeBody = (char **) malloc(MAX_ARRAY_SIZE * sizeof(char *));
    memset(syntax->codeBody, 0, MAX_ARRAY_SIZE);
    for (int i = 0; i < MAX_ARRAY_SIZE; ++i) {
        syntax->codeBody[i] = (char *) malloc(MAX_ARRAY_SIZE * sizeof(char));
        memset(syntax->codeBody[i], 0, MAX_ARRAY_SIZE);
    }
}

codeSyntax *createSyntax() {
    codeSyntax *syntax = (codeSyntax *) malloc(1 * sizeof(codeSyntax));
    initSyntax(syntax);
    return syntax;
}

int splitSyntax(char *srcFile, char **dest, char **divs) {
    FILE *in = fopen(srcFile, "r");
    if (in == NULL) {
        exit(-2);
    }
    char buffStr[MAX_E_SIZE];
    int syntaxIndex = 0;
    while (fgets(buffStr, MAX_E_SIZE, in)) {

        int i = 0;
        while (buffStr[i] != '\n') {

            while (i < strlen(buffStr) - 2 && (buffStr[i + 1] == ' ' || buffStr[i + 1] == '\t')) i++;

            int y = 0;
            int divisionFlag = 0;
            int dividerId = 0;

            for (int j = 0; j < 100; j++) {
                char *charsBuffer;
                charsBuffer = (char *) malloc(strlen(divs[j]) * sizeof(char));
                for (int z = 0; z < strlen(divs[j]); z++) {
                    charsBuffer[z] = buffStr[i + z];
                }
                charsBuffer[strlen(divs[j])] = '\0';
                divisionFlag = !strcmp(charsBuffer, divs[j]);
                free(charsBuffer);
                if (divisionFlag) {
                    dividerId = j;
                    break;
                }
            }

            if (!divisionFlag) {
                dest[syntaxIndex][y++] = buffStr[i++];
            } else {
                y = 0;
                syntaxIndex++;
                strcpy(dest[syntaxIndex++], divs[dividerId]);
                i += strlen(divs[dividerId]) - 1;
            }

        }
    }
    return syntaxIndex;
}