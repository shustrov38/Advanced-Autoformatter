#ifndef ADVANCED_AUTOFORMATTER_PARSER_H
#define ADVANCED_AUTOFORMATTER_PARSER_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char **codeBody;
} codeSyntax;

codeSyntax *createSyntax();

int rsyntax(char *srcFile, char dest[100][100],char *divs[56]);
void destroySyntax(codeSyntax *cs);

#endif //ADVANCED_AUTOFORMATTER_PARSER_H
