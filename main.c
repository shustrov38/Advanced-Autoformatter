#include "libraries.h"
#include "parser.h"
#include "lineMaker.h"

#include "Calculator/Operations/ops.h"
#include "Calculator/RPN/RPN.h"
#include "Calculator/Stack/stack.h"
#include "Calculator/OpTree/tree.h"
#include "Calculator/Parser/parser.h"

#define MAX_STRING_LEN 20

#pragma region FileUtilities

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

int getLineLength(char **line) {
    int size = -1;
    while (line[++size][0]);
    return size;
}

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

    // CALCULATOR ALGO

    // init RPN struct
    rpnProcessor *outStack = rpnProcInit();

    // length of one code line, includes ';'
    int codeLineLength = getLineLength(files[0].code->codeLines[0]);

    int size = 0; // size - size of the Expression array
    Expression *e = createExpressions();

    // add and convert expression from code line to calculus expression
    int lineLen = addExpression(e, size++, files[0].code->codeLines[0],
                                codeLineLength); // lineLen - length of the formula

    // result stack with RPN
    Stack *stack = rpnFunc(outStack, e[0].formula, lineLen);
    stPrint(stack);
    printf("\n");

    Node *root = nodeInit();
    opTreeGen(root, stack);

    double complex ans = opTreeCalc(root, NULL, 0, 0);
    printf("result = ");
    printNum(ans);
    printf("\n");

    // TODO: remake OpTree for operations [ x++, (--x)++, etc..]
    return EXIT_SUCCESS;
}