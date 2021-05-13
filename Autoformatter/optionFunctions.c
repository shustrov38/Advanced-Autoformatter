#include "optionFunctions.h"


int isOpenBr(char *string) {
    if (!strcmp(string, "(")) {
        return 1;
    } else return 0;
}

int isCloseBr(char *string) {
    if (!strcmp(string, ")")) {
        return 1;
    } else return 0;
}

int isOpenFigBr(char *string) {
    if (!strcmp(string, "{")) {
        return 1;
    } else return 0;
}

int isCloseFigBr(char *string) {
    if (!strcmp(string, "}")) {
        return 1;
    } else return 0;
}

int isSemicolon(char *string) {
    if (!strcmp(string, ";")) {
        return 1;
    } else return 0;
}

int isStruct(char *string) {
    if (!strcmp(string, "struct") || !strcmp(string, "enum")) {
        return 1;
    } else return 0;
}

int isDo(char *string) {
    if (!strcmp(string, "do")) {
        return 1;
    } else return 0;
}

int isBlock(char *string) {
    if (!strcmp(string, "while") || !strcmp(string, "for") || !strcmp(string, "do") || !strcmp(string, "if") ||
        !strcmp(string, "switch")) {
        return 1;
    } else return 0;
}

int isDoubleSlash(char *string) {
    if (!strcmp(string, "//")) {
        return 1;
    } else return 0;
}

int isSlashAndStar(char *string) {
    if (!strcmp(string, "/*")) {
        return 1;
    } else return 0;
}