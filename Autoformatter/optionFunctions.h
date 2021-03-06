#ifndef ADVANCED_AUTOFORMATTER_OPTIONFUNCTIONS_H
#define ADVANCED_AUTOFORMATTER_OPTIONFUNCTIONS_H

#include "../libraries.h"

int isTypeData(char *string);
int isOpenBr(char *string);
int isCloseBr(char *string);
int isOpenFigBr(char *string);
int isCloseFigBr(char *string);
int isSemicolon(char *string);
int isStruct(char *string);
int isDo(char *string);
int isBlock(char *string);
int isSlashAndStar(char *string);
int isDoubleSlash(char *string);

#endif //ADVANCED_AUTOFORMATTER_OPTIONFUNCTIONS_H