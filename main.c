#include "libraries.h"
#include "parser.h"
#include "lineMaker.h"

#include "Calculator/Operations/ops.h"
#include "Calculator/RPN/RPN.h"
#include "Calculator/Stack/stack.h"
#include "Calculator/OpTree/tree.h"

#define MAX_STRING_LEN 20

#pragma region Structures

#define VARIABLES_COUNT 50
#define FUNCTIONS_COUNT 50
#define STRUCTS_COUNT 50

typedef enum {
    Int,
    Double,
    Char
} VarType;

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
    int value[VARIABLES_COUNT];
} VariablesInt;

typedef struct {
    int size;
    int inited[VARIABLES_COUNT];
    char name[MAX_STRING_LEN][VARIABLES_COUNT];
    double value[VARIABLES_COUNT];
} VariablesDouble;

typedef struct {
    int size;
    int inited[VARIABLES_COUNT];
    char name[MAX_STRING_LEN][VARIABLES_COUNT];
    char value[VARIABLES_COUNT];
} VariablesChar;

#pragma endregion Structures

#pragma region FileUtilities

#define MAX_FILES 10

typedef struct {
    char filename[MAX_STRING_LEN];
    int isHeader;
    codeLineStruct *code;
    Functions funcs;
    Structs structs;
    VariablesInt vari;
    VariablesDouble vard;
    VariablesChar varc;
} FileData;

typedef enum {
    Incorrect,
    Header,
    Source
} FileExtension;

FileExtension isCorrectFilename(const char *filename) {
    char *rightDotSymbol = strrchr(filename, '.');
    if (rightDotSymbol == NULL) {
        return Incorrect;
    }
    ++rightDotSymbol;
    if (!strcmp(rightDotSymbol, "h")) {
        return Header;
    }
    if (!strcmp(rightDotSymbol, "c")) {
        return Source;
    }
    return Incorrect;
}

#pragma endregion FileUtilities

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FileData files[MAX_FILES];
    unsigned filesCount = 0;

    const char *divs[52] = {
            "//", ">>=", "<<=", "/*", "*/", "\'",
            "+=", "-=", "*=", "/=", "==", "++", "--", ">=", "<=",
            "!=", "&&", "||", "^^", "^=", "|=", "&=", "~=", ">>", "<<",
            ">", "<", "+", "-", "*", "/", "=",
            "!", "?", "&", "|", "^", "~",
            "(", ")", "[", "]", "{", "}",
            " ", ".", ",", ";", ":", "\n", "\t", "\""
    };

    // array to store information from splitter
    char **code = (char **) malloc(MAX_CODE_LEN * sizeof(char *));
    for (int j = 0; j < MAX_CODE_LEN; ++j) {
        code[j] = (char *) malloc(MAX_DIVISOR_LEN * sizeof(char));
        memset(code[j], 0, MAX_DIVISOR_LEN);
    }

    // loop with file processing (get filename, split by lines)
    for (int i = 1; i < argc; ++i) {
        FileExtension ext = isCorrectFilename(argv[i]);
        if (ext == Incorrect) {
            printf("File \"%s\" has incorrect extension.\n", argv[i]);
            return EXIT_FAILURE;
        }
        strcpy(files[filesCount].filename, argv[1]);
        for (int j = 0; j < MAX_CODE_LEN; ++j) {
            memset(code[j], 0, MAX_DIVISOR_LEN);
        }
        int n = splitSyntax(files[filesCount].filename, code, divs);
//        printf("%d\n", n);
//        for (int j = 0; j < n; ++j) {
//            printf("%d) [%s]\n", j, code[j]);
//        }
        printf("\n");
        files[filesCount].code = createCodeLineStruct();
        splitLines(files[filesCount].code, n, code);
        ++filesCount;
    }

    // free unused memory
    for (int j = 0; j < MAX_CODE_LEN; ++j) {
        free(code[j]);
    }
    free(code);

    // print all files
    for (int i = 0; i < filesCount; ++i) {
        printf("\n%s\n", files[i].filename);
        printf("------------------------------------------------------\n");
        printCode(files[i].code);
        printf("------------------------------------------------------\n");
    }

    rpnProcessor *outStack = rpnProcInit();

    int size = 0;
    for (; files[0].code->codeLines[0][size]; ++size);

    Stack *stack = rpnFunc(outStack, files[0].code->codeLines[0], size);

    Node *root = nodeInit();
    opTreeGen(root, stack);

    double complex ans = opTreeCalc(root, NULL, 0, 0);
    printf("result = ");
    printNum(ans);
    printf("\n");

    return EXIT_SUCCESS;
}