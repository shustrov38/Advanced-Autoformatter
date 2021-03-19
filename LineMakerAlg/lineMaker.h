#ifndef ADVANCED_AUTOFORMATTER_LINEMAKER_H
#define ADVANCED_AUTOFORMATTER_LINEMAKER_H

#include "../optionFuncs/optionFunctions.h"

#include <stdio.h>
#include <malloc.h>

#define SIZE 1000
#define ONE_STRING_SIZE 1000

typedef struct {
    char ***codeLines;
    int linesCnt;
} codeLineStruct;

codeLineStruct *init();

codeLineStruct *splitStringAlgorithm(codeLineStruct *codeBody, int len, char **originString);

#endif //ADVANCED_AUTOFORMATTER_LINEMAKER_H

