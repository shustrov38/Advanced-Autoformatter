#ifndef ADVANCED_AUTOFORMATTER_LINEMAKER_H
#define ADVANCED_AUTOFORMATTER_LINEMAKER_H

#include "../libraries.h"

typedef struct {
    char ***codeLines;
    int linesCnt;
} codeLineStruct;

codeLineStruct *createCodeLineStruct();

void printCode(codeLineStruct *code);

void splitLines(codeLineStruct *codeBody, int len, char **originString);

#endif //ADVANCED_AUTOFORMATTER_LINEMAKER_H

// TODO: 1)Двойные комментарии вроде работают, но не факт что везде
//  2)Скорее всего надо еще пофиксить пробелы
//  3)Форматирование в файл



