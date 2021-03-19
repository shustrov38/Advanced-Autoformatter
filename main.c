#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 10
#define VARIABLES_COUNT 50
#define FUNCTIONS_COUNT 50
#define STRUCTS_COUNT 50
#define MAX_ARRAY_LEN 50

typedef enum {
    Int,
    Double,
    Char
} VarType;

typedef struct {
    int size;
    char *name[MAX_STRING_LEN];
} CodeStructs;

typedef struct {
    int size;
    int inited[STRUCTS_COUNT];
    char name[MAX_STRING_LEN][STRUCTS_COUNT];
    int fieldCnt[STRUCTS_COUNT];
} Structs;

typedef struct {
    int size;
    int used[FUNCTIONS_COUNT];
    char name[MAX_STRING_LEN][FUNCTIONS_COUNT];
} Functions;

typedef struct {
    int size;
    int inited[VARIABLES_COUNT];
    char name[MAX_STRING_LEN][VARIABLES_COUNT];
    VarType type[VARIABLES_COUNT];
    int value_int[VARIABLES_COUNT];
    double value_double[VARIABLES_COUNT];
    char value_char[VARIABLES_COUNT];
} Variables;

typedef struct segment_t {
    int isCommented;
    int isBlock;
    char *name;
    char code[MAX_STRING_LEN][MAX_ARRAY_LEN];
    Structs structs;
    Variables variables;
    struct segment_t *subBlock;
} Segment;

int haveFunction(Functions *functions, char *str) {
    int n = functions->size;
    for (int i = 0; i < n; ++i) {
        if (!strcmp(functions->name[i], str)) {
            return 1;
        }
    }
    return 0;
}

int isBlock(Functions *functions, CodeStructs *codeStructs, char *str) {
    int n = codeStructs->size;
    for (int i = 0; i < n; ++i) {
        if (!strcmp(codeStructs->name[i], str)) {
            return 1;
        }
    }
    return haveFunction(functions, str);
}

// TODO: generate tree function based on string array
//void genCodeTree(Segment *root, ) {
//
//}

int main() {
    CodeStructs codeStructs = {6, {"if", "do", "for", "while", "else", "switch"}};

    // dumpFunctions()

    // TODO: "int a;    // hello world!" -> ["int a;", "// hello world!"]

    return EXIT_SUCCESS;
}
