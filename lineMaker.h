#ifndef ADVANCED_AUTOFORMATTER_LINEMAKER_H
#define ADVANCED_AUTOFORMATTER_LINEMAKER_H

#include "libraries.h"

typedef struct {
    char ***codeLines;
    int linesCnt;
} codeLineStruct;

codeLineStruct *createCodeLineStruct();

void splitLines(codeLineStruct *codeBody, int len, char **originString);

#endif //ADVANCED_AUTOFORMATTER_LINEMAKER_H

