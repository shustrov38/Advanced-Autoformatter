#include "libraries.h"
#include "Autoformatter/parser.h"
#include "Autoformatter/lineMaker.h"

#include "Calculator/ops.h"
#include "Calculator/RPN.h"
#include "Calculator/stack.h"
#include "Calculator/tree.h"
#include "Calculator/parser.h"

#include "vector.h"
#include "memory.h"

#define MAX_STRING_LEN 20

int getLineLength(char **line) {
    int size = -1;
    while (line[++size][0]);
    return size;
}

#pragma region FileUtilities

#define MAX_FILES 10

typedef struct {
    char name[MAX_STRING_LEN];

    int inputsCount;
    Variant inputs[ARRAY_LEN];
    char input_names[MAX_STRING_LEN][ARRAY_LEN];
    Variant output;

    codeLineStruct *code;

    int nestedCount;
    char nestedFunctions[MAX_STRING_LEN][ARRAY_LEN];
} Function;

void initFunction(Function *f) {
    memset(f->name, 0, MAX_STRING_LEN);

    f->inputsCount = 0;
    for (int i = 0; i < ARRAY_LEN; ++i) {
        memset(f->nestedFunctions[i], 0, MAX_STRING_LEN);
    }

    f->code = createCodeLineStruct();

    f->nestedCount = 0;
    for (int i = 0; i < ARRAY_LEN; ++i) {
        memset(f->nestedFunctions[i], 0, MAX_STRING_LEN);
    }
}

typedef struct {
    char filename[MAX_STRING_LEN];
    int isHeader;
    Function functions[ARRAY_LEN];
    codeLineStruct *code;
} FileData;

int loadFunctions(FileData *file) {
    int size = 0;
    for (int k = 0; k < file->code->linesCnt; ++k) {
        char **line = file->code->codeLines[k];
        int length = getLineLength(line);

        // TODO: find way to move type checking into function
        if (length > 1 && !strcmp(line[0], "char") && !strcmp(line[1], "*")) {
            file->functions[size].output.varType = String;
        } else {
            file->functions[size].output.varType = Numerical;
            int isUnsigned = !strcmp(line[1], "unsigned");

            int isDouble = !strcmp(line[0], "float") ||
                           !strcmp(line[0], "double") ||
                           length > 1 && !strcmp(line[0], "long") && !strcmp(line[1], "double");

            if (isUnsigned) {
                file->functions[size].output.numType = Unsigned;
            } else if (isDouble) {
                file->functions[size].output.numType = Double;
            } else {
                file->functions[size].output.numType = Int;
            }
        }

        // check if its function
        int i = 0;
        while (!strcmp(line[i], "(") && i < length) ++i;

        if (i == length) {
            // its variable definition
            continue;
        } else {
            strcpy(file->functions[size].name, line[i - 1]);
            // TODO: store all inputs for current function
        }

        // store function body
        while (!strcmp(file->code->codeLines[++k][0], "}")) {
            line = file->code->codeLines[k];
            length = getLineLength(line);

            // find nested
            if (length > 1) {
                // TODO: check if line[0] not for, switch, if and etc..
                strcpy(file->functions[size].nestedFunctions[file->functions[size].nestedCount++], line[0]);
            } else if (length > 2) {
                strcpy(file->functions[size].nestedFunctions[file->functions[size].nestedCount++], line[1]);
            }

            // copy loop
            for (i = 0; i < length + 1; ++i) {
                strcpy(file->functions[size].code->codeLines[file->functions[size].code->linesCnt][i], line[i]);
                file->functions[size].code->linesCnt++;
            }
        }

        ++size;
    }
    return size;
}

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
        strcpy(files[i-1].filename, argv[i]);
        int n = splitSyntax(argv[i], code);
//        printf("%d\n", n);
//        for (int j = 0; j < n; ++j) {
//            printf("%d) [%s]\n", j, code[j]);
//        }
//        for (int j = 0; j < ARRAY_LEN; ++j) {
//            initFunction(&files[i - 1].functions[j]);
//        }
        printf("\n");
        files[i - 1].code = createCodeLineStruct();
        splitLines(files[i - 1].code, n, code);

    }

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

Expression *interpretFile(Memory *m, FileData *file) {
    // size of the Expression array
    int size = 0;
    Expression *e = createExpressions();
    Stack *meta = stCreate();

    // TODO: it might be better to start the interpretation directly from the main function,
    //  but all global variables must be stored.

    // TODO: got bug, if there is no semicolon at the end of the code line, the expression is not applied.

    // TODO: change cycle, choose only arithmetic lines of code.
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        int q = addExpression(e, size++, file->code->codeLines[i], codeLineLength, meta, i);
        for (int qq = 0; qq < q; ++qq) {
            printf("%s ", e[i].code[qq]);
        }
        printf("\n");
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

//    int count = loadFunctions(&files[0]);
//    printf("functions count: %d\n", count);

//    // CALCULATOR ALGO
//
//    INIT_MEMORY(m);
//
//    MEMORY_NEW_NUM(m, Int, "X", 1);
//    MEMORY_NEW_NUM(m, Double, "Y", 2.7);
//    MEMORY_NEW_NUM(m, Unsigned, "Z", -3);
//    MEMORY_NEW_STR(m, "S", "H3110_WR1D");
//
//    printf("Variables before interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    MemoryFunctions.printRegister(&m, String);
//    printf("\n");
//
//    Expression *e = interpretFile(&m, &files[0]);
//
//    printf("Variables after interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    MemoryFunctions.printRegister(&m, String);
//    printf("\n");

    return EXIT_SUCCESS;
}