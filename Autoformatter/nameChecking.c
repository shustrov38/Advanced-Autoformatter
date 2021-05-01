#include "nameChecking.h"
#include "../fileUtils.h"

#define VARSIZE 50
#define FUNCSIZE 30
#define TYPESIZE 20
#define ELSIZE 30

//Для объявления переменных должен использоваться camelStyle,
//для объявления собственных типов (псевдонимов) и функций - PascalStyle.

//Неспользуемые переменные и функции

void outputUnused(int *numbers, char **names, int cnt, int flag){
    int k = 0;
    for (int i = 0; i < cnt; ++i){
        if (!numbers[i]){
            printf("{ %s } ", names[i]);
            k++;
        }
    }
    if (!k){
        if (flag == 1) printf("No unused vars");
        if (flag == 2) printf("No unused funcs");
    }
    printf("\n");
}

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
            return i;
        }
    }
    return -1;
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
    char **varNames = (char **) malloc(VARSIZE * sizeof(char *));
    for (int i = 0; i < VARSIZE; ++i) {
        varNames[i] = (char *) malloc(ELSIZE * sizeof(char));
    }
    int varNamesCnt = 0;


    //funcNames
    char **funcNames = (char **) malloc(FUNCSIZE * sizeof(char *));
    for (int i = 0; i < FUNCSIZE; ++i) {
        funcNames[i] = (char *) malloc(ELSIZE * sizeof(char));
    }
    int funcNamesCnt = 0;


    //newTypeData
    char **newTypes = (char **) malloc(TYPESIZE * sizeof(char *));
    for (int i = 0; i < TYPESIZE; ++i) {
        newTypes[i] = (char *) malloc(ELSIZE * sizeof(char));
    }
    int newTypesCnt = 0;


    //varNumbers
    int *varNumbers = (int *) malloc(VARSIZE * sizeof(int));
    for (int i = 0; i < VARSIZE; ++i){
        varNumbers[i] = 0;
    }

    //funcNumbers
    int *funcNumbers = (int *) malloc(FUNCSIZE * sizeof(int));
    for (int i = 0; i < FUNCSIZE; ++i){
        funcNumbers[i] = 0;
    }


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
                    if (checkUnique(varNames, code->codeLines[i][s + 1], varNamesCnt) == -1) {
                        strcpy(varNames[varNamesCnt], code->codeLines[i][s + 1]);
                        varNamesCnt++;
                        total += checkCamelCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }

                //func
                if (!strcmp(code->codeLines[i][s + 2], "(")) {
                    if (checkUnique(funcNames, code->codeLines[i][s + 1], funcNamesCnt) == -1) {
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
                if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt) == -1) {
                    strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                    newTypesCnt++;
                    total += checkPascalCase(code->codeLines[i][s + 1], i);
                }
                s++;
                continue;
            }

            if (cmpUserTypes(code->codeLines[i][s], newTypes, newTypesCnt)) {
                if (!strcmp(code->codeLines[i][s + 2], "(")) {
                    if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt) == -1) {
                        strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                        newTypesCnt++;
                        total += checkPascalCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }

                if (!strcmp(code->codeLines[i][s + 2], ";") || !strcmp(code->codeLines[i][s + 2], "=")
                    || !strcmp(code->codeLines[i][s + 2], "[")) {
                    if (checkUnique(newTypes, code->codeLines[i][s + 1], newTypesCnt) == -1) {
                        strcpy(newTypes[newTypesCnt], code->codeLines[i][s + 1]);
                        newTypesCnt++;
                        total += checkCamelCase(code->codeLines[i][s + 1], i);
                    }
                    s += 2;
                    continue;
                }
            }

            if (checkUnique(varNames, code->codeLines[i][s], varNamesCnt) != -1){
                varNumbers[checkUnique(varNames, code->codeLines[i][s], varNamesCnt)]++;
                continue;
            }

            if (checkUnique(funcNames, code->codeLines[i][s], funcNamesCnt) != -1){
                funcNumbers[checkUnique(funcNames, code->codeLines[i][s], funcNamesCnt)]++;
                continue;
            }
        }
    }

    printf("\n");
    printf("Unused variables: ");
    outputUnused(varNumbers, varNames, varNamesCnt, 1);
    printf("Unused functions: ");
    outputUnused(funcNumbers, funcNames, funcNamesCnt, 2);


    if (total == 0) {
        printf("This file has not got any problems with variable/function names.\n");
    }
    printf("\n");
}