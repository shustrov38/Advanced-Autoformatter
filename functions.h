#ifndef ADVANCED_AUTOFORMATTER_FUNCTIONS_H
#define ADVANCED_AUTOFORMATTER_FUNCTIONS_H

#include "libraries.h"
#include "Autoformatter/lineMaker.h"
#include "memory.h"

typedef struct {
    char name[MAX_STRING_LEN];

    int inputsCount;
    Variant inputs[ARRAY_LEN];
    char input_names[MAX_STRING_LEN][ARRAY_LEN];
    Variant output;

    codeLineStruct *code;

    int nestedCount;
    char nestedFunctions[MAX_STRING_LEN][ARRAY_LEN];
} Function;

Function *createFunctions();

#endif //ADVANCED_AUTOFORMATTER_FUNCTIONS_H
