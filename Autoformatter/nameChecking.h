#ifndef ADVANCED_AUTOFORMATTER_NAMECHECKING_H
#define ADVANCED_AUTOFORMATTER_NAMECHECKING_H

#include "lineMaker.h"
#include "optionFunctions.h"


void checkNames(codeLineStruct *code);
int checkUnique(char **namesArray, char* name, int cnt);
void checkCamelCase(char *name, int pos);
void checkPascalCase(char *name, int pos);

#endif //ADVANCED_AUTOFORMATTER_NAMECHECKING_H
