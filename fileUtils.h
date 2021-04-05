#ifndef ADVANCED_AUTOFORMATTER_FILEUTILS_H
#define ADVANCED_AUTOFORMATTER_FILEUTILS_H

#include "libraries.h"
#include "functions.h"
#include "Autoformatter/lineMaker.h"
#include "Autoformatter/parser.h"

typedef struct {
    char filename[MAX_STRING_LEN];
    int isHeader;
    Function *functions;
    int funcCount;
    codeLineStruct *code;
} FileData;

// return length of code line
int getLineLength(char **line);

// loads all functions from file
void loadFunctions(FileData *file);

// loads all files from argv
int loadFiles(FileData *files, int argc, const char *argv[]);

// prints all stored files
void printAllFiles(FileData *files, int size);

// prints all stored functions
void printAllFunctions(FileData *file);

#endif //ADVANCED_AUTOFORMATTER_FILEUTILS_H
