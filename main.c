#include "libraries.h"
#include "parser.h"
#include "lineMaker.h"

#pragma region FileUtilities

#define MAX_STRING_LEN 20
#define MAX_FILES 10

typedef struct {
    char filename[MAX_STRING_LEN];
    int isHeader;
    codeLineStruct *code;
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

#pragma region FunctionUtilities

#define FUNCTIONS_COUNT 50

struct functions_t {
    int size;
    int used[FUNCTIONS_COUNT];
    char name[MAX_STRING_LEN][FUNCTIONS_COUNT];
} Functions;

#pragma endregion FunctionUtilities

//TODO:
//1) Parser: skip special symbols (\t, space), comment + string, broken CRLF
//2) LineMaker: includes, enums, defines

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FileData files[MAX_FILES];
    unsigned filesCount = 0;

    const char *divs[52] = {
            "//", ">>=", "<<=", "/*", "*/","'",
            "+=", "-=", "*=", "/=", "==", "++", "--", ">=", "<=",
            "!=", "&&", "||", "^^", "^=", "|=", "&=", "~=", ">>", "<<",
            ">", "<", "+", "-", "*", "/", "=",
            "!", "?", "&", "|", "^", "~",
            "(", ")", "[", "]", "{", "}",
            " ", ".", ",", ";", ":", "\n", "\t", "\""
    };

    // array to store information from splitter
    char **code = (char **)malloc(MAX_CODE_LEN * sizeof (char*));
    for (int j = 0; j < MAX_CODE_LEN; ++j){
        code[j] = (char *) malloc (MAX_DIVISOR_LEN * sizeof(char));
        memset(code[j],0,MAX_DIVISOR_LEN);
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
        printf("%d",n);
        for (int j = 0; j <= n; ++j) {
            printf("%d) [%s]\n", j, code[j]);
        }
        printf("\n");
        files[filesCount].code = createCodeLineStruct();
        splitLines(files[filesCount].code, n+1, code);
        ++filesCount;
    }

    // free unused memory
    for (int j = 0; j < MAX_CODE_LEN; ++j) {
        free(code[j]);
    }
    free(code);

    // print all files by lines
    for (int i = 0; i < filesCount; ++i) {
        printf("%d\n", files[i].code->linesCnt);
        for (int j = 0; j < files[i].code->linesCnt; ++j) {
            for (int k = 0; files[i].code->codeLines[j][k]; ++k) {
                printf("%s ", files[i].code->codeLines[j][k]);
            }
            printf("\n");
        }
    }

    return EXIT_SUCCESS;
}