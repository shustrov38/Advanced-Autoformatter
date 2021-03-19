#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LineMakerAlg/lineMaker.h"

int main() {
    char **exampleArr = (char**)malloc(SIZE*sizeof(char*));
    for (int j = 0; j < ONE_STRING_SIZE; ++j){
        exampleArr[j] = (char*)malloc(ONE_STRING_SIZE*sizeof(char));
    }

    for (int i = 0; i < 57; ++i){
        scanf("%s", exampleArr[i]);
    }

    int len = 57;

    codeLineStruct *body = init();

    splitStringAlgorithm(body, len, exampleArr);


    return EXIT_SUCCESS;
}
