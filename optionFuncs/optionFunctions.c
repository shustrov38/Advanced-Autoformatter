#include "optionFunctions.h"

int isOpenBr(char *string){
    if (!strcmp(string, "(")){
        return 1;
    } else return 0;
}

int isCloseBr(char *string){
    if (!strcmp(string, ")")){
        return 1;
    } else return 0;
}

int isOpenFigBr(char *string){
    if (!strcmp(string, "{")){
        return 1;
    } else return 0;
}

int isCloseFigBr(char *string){
    if (!strcmp(string, "}")){
        return 1;
    } else return 0;
}

int isSemicolon(char *string){
    if (!strcmp(string, ";")){
        return 1;
    } else return 0;
}

int isStruct(char *string){
    if (!strcmp(string, "struct")){
        return 1;
    } else return 0;
}




