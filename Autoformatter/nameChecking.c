#include "nameChecking.h"
#include "../fileUtils.h"

//Для объявления переменных должен использоваться camelStyle,
//для объявления собственных типов (псевдонимов) и функций - PascalStyle.


typedef struct{
    char *name;
    int usingCnt; //счетчик использований
    int type; //1 - var; 2 - func
} allNames;

int compareTypes(char* string, char **vocab, int vocabCnt){
    for (int i = 0; i < vocabCnt; ++i){
        if (!strcmp(string, vocab[i])){
            return i;
        }
    }
    return -1;
}

int checkUnique(allNames *namesArray, int cnt, char *name) {
    for (int i = 0; i < cnt; ++i) {
        if (!strcmp(name, namesArray[i].name)) {
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
    printf("File \"%s\":\n\n", fileName);
    int total = 0;

    //vars/funcs names
    allNames *names = (allNames *) malloc(100 * sizeof(allNames));
    for (int i = 0; i < 100; ++i){
        names[i].name = (char *) malloc(50 * sizeof(char));
        names[i].usingCnt = 0;
        names[i].type = 0;
    }
    int namesCnt = 0;

    //types' vocabulary
    char **types = (char **) malloc(20 * sizeof(char*));
    for (int i = 0; i < 20; ++i){
        types[i] = (char *) malloc(50 * sizeof(char));
    }
    int typesCnt = 5;
    strcpy(types[0], "int");
    strcpy(types[1], "char");
    strcpy(types[2], "double");
    strcpy(types[3], "float");
    strcpy(types[4], "void");


    for (int i = 0; i < code->linesCnt; ++i) {
        int len = getLineLength(code->codeLines[i]);
        int typeNum = -2;
        int varNum = -2;

        for (int s = 0; s < len; ++s) {
            if (!strcmp(code->codeLines[i][s], "struct")){
                strcpy(types[typesCnt], code->codeLines[i][s + 1]);
                if (checkPascalCase(code->codeLines[i][s + 1], i)) total++;
                typesCnt++;
            }

            typeNum = compareTypes(code->codeLines[i][s], types, typesCnt);

            if (typeNum != -1){
                //exception
                if (!strcmp(code->codeLines[i][s + 1], "main")){
                    continue;
                }

                if (checkUnique(names, namesCnt, code->codeLines[i][s + 1]) == -1) {
                    strcpy(names[namesCnt].name, code->codeLines[i][s + 1]);


                    if (!strcmp(code->codeLines[i][s + 2], "(")) {
                        if (checkPascalCase(code->codeLines[i][s + 1], i)) total++;
                        names[namesCnt].type = 2;
                        namesCnt++;
                    } else {
                        if (checkCamelCase(code->codeLines[i][s + 1], i)) total++;
                        names[namesCnt].type = 1;
                        namesCnt++;
                    }
                    s += 2;
                }
                continue;
            }

            varNum = checkUnique(names, namesCnt, code->codeLines[i][s]);
            if (varNum != -1){
                names[varNum].usingCnt++;
                continue;
            }
        }
    }


    if (total) {
        printf("\n");
    }


    for (int i = 0; i < namesCnt; ++i){
        if (names[i].usingCnt == 0){
            if (names[i].type == 1){
                printf("Variable { %s } wasn't used after creating\n", names[i].name);
            } else {
                printf("Function { %s } wasn't used after creating\n", names[i].name);
            }
        }
    }
    printf("\n");


    if (total == 0) {
        printf("This file has not got any problems with variable/function names.\n");
    }

    printf("\n");
}

