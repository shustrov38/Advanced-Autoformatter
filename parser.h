#ifndef ADVANCED_AUTOFORMATTER_PARSER_H
#define ADVANCED_AUTOFORMATTER_PARSER_H
#define MAX_ARRAY_SIZE 10000

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    char **codeBody;
} codeSyntax;

codeSyntax *createSyntax();

int splitSyntax(char *srcFile, char **dest, char *divs[56]);

void destroySyntax(codeSyntax *cs);

#endif //ADVANCED_AUTOFORMATTER_PARSER_H
