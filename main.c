#include "libraries.h"
#include "Autoformatter/parser.h"
#include "Autoformatter/lineMaker.h"

#include "Calculator/ops.h"
#include "Calculator/RPN.h"
#include "Calculator/stack.h"
#include "Calculator/tree.h"
#include "Calculator/parser.h"

#include "memory.h"

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

int loadFiles(FileData *files, int argc, const char *argv[]) {
    // TODO: move divs to parser.c and remove magic number 52, maybe #define.
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
        strcpy(files[i-1].filename, argv[1]);
        for (int j = 0; j < MAX_CODE_LEN; ++j) {
            memset(code[j], 0, MAX_DIVISOR_LEN);
        }
        int n = splitSyntax(files[i-1].filename, code, divs);
        //DEBUG
//        printf("%d\n", n);
//        for (int j = 0; j < n; ++j) {
//            printf("%d) [%s]\n", j, code[j]);
//        }
        //DEBUG
        printf("\n");
        files[i-1].code = createCodeLineStruct();
        splitLines(files[i-1].code, n, code);
    }

    // free unused memory
    for (int j = 0; j < MAX_CODE_LEN; ++j) {
        free(code[j]);
    }
    free(code);

    return argc - 1;
}

// prints non-formatted code of each file to the console
void printAllFiles(FileData *files, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%s\n", files[i].filename);
        printf("------------------------------------------------------\n");
        printCode(files[i].code);
        printf("------------------------------------------------------\n");
        printf("\n");
    }
}

#pragma endregion FileUtilities

int getLineLength(char **line) {
    int size = -1;
    while (line[++size][0]);
    return size;
}

Expression *interpretFile(Memory *m, FileData *file) {
    // size of the Expression array
    int size = 0;
    Expression *e = createExpressions();

    // TODO: it might be better to start the interpretation directly from the main function,
    //  but all global variables must be stored.

    // TODO: got bug, if there is no semicolon at the end of the code line, the expression is not applied.

    // TODO: change cycle, choose only arithmetic lines of code.
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        addExpression(e, size++, file->code->codeLines[i], codeLineLength);
    }

    // iterate through Expressions and interpret each of them
    for (int i = 0; i < size; ++i) {
        rpnProcessor *outStack = rpnProcInit();

        // result stack with RPN
        Stack *stack = rpnFunc(outStack, e[i].code, e[i].size);

        Node *root = nodeInit();
        opTreeGen(root, stack);
        opTreeCalc(m, root);

        // after each calculation
        MemoryFunctions.overflowCheck(m);
    }

    return e;
}

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FileData files[MAX_FILES];
    int filesCount = loadFiles(files, argc, argv);
    printAllFiles(files, filesCount);

    // CALCULATOR ALGO

    INIT_MEMORY(m);

    MEMORY_NEW_NUM(m, Int, "a", 1);
    MEMORY_NEW_NUM(m, Int, "b", 2);
    MEMORY_NEW_NUM(m, Int, "c", 3);

    printf("Variables before interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    printf("\n");

    Expression *e = interpretFile(&m, &files[0]);

    printf("Variables after interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    printf("\n");

    return EXIT_SUCCESS;
}