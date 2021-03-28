#ifndef ADVANCED_AUTOFORMATTER_PARSER_H
#define ADVANCED_AUTOFORMATTER_PARSER_H

#include "libraries.h"

#define MAX_CODE_LEN 10000
#define MAX_DIVISOR_LEN 100

typedef struct {
    char **codeBody;
} codeSyntax;

codeSyntax *createSyntax();

int splitSyntax(char *srcFile, char **dest, const char *divs[52]);

void destroySyntax(codeSyntax *cs);

#endif //ADVANCED_AUTOFORMATTER_PARSER_H
