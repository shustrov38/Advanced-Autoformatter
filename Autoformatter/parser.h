#ifndef ADVANCED_AUTOFORMATTER_PARSER_H
#define ADVANCED_AUTOFORMATTER_PARSER_H

#include "../libraries.h"

#define MAX_CODE_LEN 10000
#define MAX_DIVISOR_LEN 100
#define DIVISION_DIVERSITY 52

typedef struct {
    char **codeBody;
} codeSyntax;

codeSyntax *createSyntax();

int splitSyntax(char *srcFile, char **dest);

void destroySyntax(codeSyntax *cs);

#endif //ADVANCED_AUTOFORMATTER_PARSER_H
