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

#include "fileUtils.h"

Expression *interpretFile(Memory *m, FileData *file) {
    // size of the Expression array
    int size = 0;
    int reqNum = 0;
    Expression *e = createExpressions();
    Stack *meta = stCreate();
    INIT_VECTOR(exeSt);
    INIT_VECTOR(exeStCap);

    // TODO: it might be better to start the interpretation directly from the main function,
    //  but all global variables must be stored.

    // TODO: got bug, if there is no semicolon at the end of the code line, the expression is not applied.

    // TODO: change cycle, choose only arithmetic lines of code.
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        int q = addExpression(e, size, file->code->codeLines[i], codeLineLength, meta, i, &exeSt, &exeStCap);
        size+=q;
    }
    size++;
    for(int y = 0; y<size;y++){
        printf("\n");
        for(int z = 0; z <e[y].size; z++){
            printf(" %s",e[y].code[z]);
        }
    }

    // iterate through Expressions and interpret each of them
    for (int i = 0; i < size && e[i].size > 0; ++i) {
        rpnProcessor *outStack = rpnProcInit();

        // result stack with RPN
        Stack *stack = rpnFunc(outStack, e[i].code, e[i].size);
        Node *root = nodeInit();
        opTreeGen(root, stack);
        opTreeCalc(m, root);

        // after each calculation
        MemoryFunctions.overflowCheck(m);
    }

    for(int zzz = 0; zzz < exeSt.total; zzz++){
        printf("\n %s", (char *) exeSt.items[zzz]);
    }

    return e;
}

int main(const int argc, const char *argv[]) {
    // TODO global:
    //  1) CLEAN UP USELESS MEMORY - there are a lot of forgotten and floating pointers
    //  2) finish work with the interpreter
    //  3) close todos with functions

    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FileData files[MAX_FILES];
    int filesCount = loadFiles(files, argc, argv);
    printAllFiles(files, filesCount);
//
//    for (int i = 0; i < filesCount; ++i) {
//        loadFunctions(&files[i]);
//        printAllFunctions(&files[0]);
//    }
//    printFunctionsCallTable(files, filesCount);

//    // CALCULATOR ALGO
//
    INIT_MEMORY(m);

    MEMORY_NEW_NUM(m, Int, "X", 1);
    MEMORY_NEW_NUM(m, Double, "Y", 2.7);
    MEMORY_NEW_NUM(m, Unsigned, "Z", -3);
    MEMORY_NEW_STR(m, "S", "H3110_WR1D");

//    printf("Variables before interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    MemoryFunctions.printRegister(&m, String);
//    printf("\n");

    Expression *e = interpretFile(&m, &files[0]);

//    printf("Variables after interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    MemoryFunctions.printRegister(&m, String);
//    printf("\n");

    return EXIT_SUCCESS;
}