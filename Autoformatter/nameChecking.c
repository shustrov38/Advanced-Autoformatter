#include "nameChecking.h"
#include "../fileUtils.h"

//Для объявления переменных должен использоваться camelStyle,
//для объявления собственных типов (псевдонимов) и функций - PascalStyle.

int cmpUserTypes(char *name, char **newTypes, int typesCnt) {
    for (int i = 0; i < typesCnt; ++i) {
        if (!strcmp(newTypes[i], name)) {
            return 1;
        }
    }
    return 0;
}

int checkUnique(char **namesArray, char *name, int cnt) {
    for (int i = 0; i < cnt; ++i) {
        if (!strcmp(name, namesArray[i])) {
            return 0;
        }
    }
    return 1;
}

int checkCamelCase(char *name, int pos) {
    if (name[0] >= 'A' && name[0] <= 'Z') {
        printf("Line %d:  Incorrect name of variable: Name { %s }\n", pos + 1, name);
        return 1;
    }

    for (int i = 1; i < strlen(name) - 1; ++i) {
        if ((name[i] >= 'A' && name[i] <= 'Z') && (name[i + 1] >= 'A' && name[i + 1] <= 'Z')) {
            printf("Line %d:  Incorrect name of variable: Name { %s }\n", pos + 1, name);
            return 1;
        }
    }
    return 0;
}

int checkPascalCase(char *name, int pos) {
    if (name[0] >= 'a' && name[0] <= 'z') {
        printf("Line %d:  Incorrect name of function or user define type name: Name { %s }\n", pos + 1, name);
        return 1;
    }

    for (int i = 1; i < strlen(name) - 1; ++i) {
        if ((name[i] >= 'A' && name[i] <= 'Z') && (name[i + 1] >= 'A' && name[i + 1] <= 'Z')) {
            printf("Line %d:  Incorrect name of function or user define type name: Name { %s }\n", pos + 1, name);
            return 1;
        }
    }
    return 0;
}


void checkNames(char *fileName, codeLineStruct *code) {
    printf("File \"%s\":\n", fileName);
    int total = 0;
    //varNames
    char **varNames = (char **) malloc(50 * sizeof(char *));
    for (int i = 0; i < 50; ++i) {
        varNames[i] = (char *) malloc(30 * sizeof(char));
    }
    int varNamesCnt = 0;

    char **funcNames = (char **) malloc(30 * sizeof(char *));
    for (int i = 0; i < 30; ++i) {
        funcNames[i] = (char *) malloc(30 * sizeof(char));
    }
    int funcNamesCnt = 0;

    //newTypeData
    char **newTypes = (char **) malloc(20 * sizeof(char *));
    for (int i = 0; i < 20; ++i) {
        newTypes[i] = (char *) malloc(30 * sizeof(char));
    }
    int newTypesCnt = 0;


    for (int i = 0; i < code->linesCnt; ++i) {
        int len = getLineLength(code->codeLines[i]);

        for (int s = 0; s < len; ++s) {
            if (isTypeData(code->codeLines[i][s])) {

                //exceptions
                if (!strcmp(code->codeLines[i][s + 1], "main")) {
                    s++;
                    continue;
                }

                //variable
                if (!strcmp(code->codeLines[i][s + 2], ";") || !strcmp(code->codeLines[i][s + 2], "=")
                    || !strcmp(code->codeLines[i][s + 2], "[")) {
                    if (checkUnique(varNames, code->codeLines[i][s + 1], varNamesCnt)) {
                        strcpy(varNames[varNamesCnt], code->codeLines[i][s + 1]);
                        varNamesCnt++;
                        total += checkCamelCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }

                //func
                if (!strcmp(code->codeLines[i][s + 2], "(")) {
                    if (checkUnique(funcNames, code->codeLines[i][s + 1], funcNamesCnt)) {
                        strcpy(funcNames[funcNamesCnt], code->codeLines[i][s + 1]);
                        funcNamesCnt++;
                        total += checkPascalCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }
            }

            //struct
            if (!strcmp(code->codeLines[i][s], "struct")) {
                if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt)) {
                    strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                    newTypesCnt++;
                    total += checkPascalCase(code->codeLines[i][s + 1], i);
                }
                s++;
                continue;
            }

            if (cmpUserTypes(code->codeLines[i][s], newTypes, newTypesCnt)) {
                if (!strcmp(code->codeLines[i][s + 2], "(")) {
                    if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt)) {
                        strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                        newTypesCnt++;
                        total += checkPascalCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }

                if (!strcmp(code->codeLines[i][s + 2], ";") || !strcmp(code->codeLines[i][s + 2], "=")
                    || !strcmp(code->codeLines[i][s + 2], "[")) {
                    if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt)) {
                        strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                        newTypesCnt++;
                        total += checkCamelCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }
            }
        }
    }
    if (total == 0) {
        printf("This file has not got any problems with variable/function names.\n");
    }
    printf("\n");
}