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
    INIT_VECTOR(reqSize);

    // TODO: it might be better to start the interpretation directly from the main function,
    //  but all global variables must be stored.

    // TODO: got bug, if there is no semicolon at the end of the code line, the expression is not applied.

    // TODO: change cycle, choose only arithmetic lines of code.
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        int q = addExpression(e, size, file->code->codeLines[i], codeLineLength, meta, i, &exeSt, &reqSize);
        size+=q;
    }
    size++;
//    for(int y = 0; y<20;y++){
//        printf("\n");
//        for(int z = 0; z <e[y].size; z++){
//            printf(" %s",e[y].code[z]);
//        }
//    }

    // iterate through Expressions and interpret each of them
    for (int i = 0; i < 20; ++i) {
        rpnProcessor *outStack = rpnProcInit();

        printf("\n");
        for (int z = 0; z < e[i].size; z++) {
            printf(" %s", e[i].code[z]);
        }

        //GOTO & tags logic

        if (!strcmp(e[i].code[0], "endof")) { //TODO: make a NORMAL notIF condition
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            if (MemoryFunctions.getValue(m, e[i].code[1])->d > 0) {
                int executionLineNum = i;
                while (strcmp(e[executionLineNum].code[0], e[i].code[1])) {
                    executionLineNum--;
                }
                i = executionLineNum;
            } else {
                i++;
            }

        } else if (!strcmp(e[i].code[0],"begin")){
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            if(MemoryFunctions.getValue(m, e[i].code[1])->d == 0){
                int executionLineNum = i;
                while(!(!strcmp(e[executionLineNum].code[0],"endof") && !strcmp(e[executionLineNum].code[1],e[i].code[1]))){
                    executionLineNum++;
                }
                i = executionLineNum;
            }

            else{
                i++;
            }

        }else if(e[i].code[0][0] == 'i' && e[i].code[0][1] == 'f'){
            if (MemoryFunctions.getValue(m, e[i].code[0])->d > 0){
                i++;
            } else {
                int executionLineNum = i;
                while(strcmp(e[executionLineNum].code[1],e[i].code[0]) && strcmp(e[executionLineNum].code[0],"end of")){
                    executionLineNum++;
                }
                i = executionLineNum-1;
            }
        }

        //INIT logics
        if (!strcmp(e[i].code[0],"int")){
            MEMORY_NEW_NUM(*m, Int, e[i].code[1], 0);
        } else if (!strcmp(e[i].code[0],"double")){
            MEMORY_NEW_NUM(*m, Double, e[i].code[1], 0);
        } else if (!strcmp(e[i].code[0],"unsigned")){
            MEMORY_NEW_NUM(*m, Unsigned, e[i].code[1], 0);
        }

        // result stack with RPN
        Stack *stack = rpnFunc(outStack, e[i].code, e[i].size);
        Node *root = nodeInit();
        opTreeGen(root, stack);
        opTreeCalc(m, root);
        // after each calculation
        MemoryFunctions.overflowCheck(m);

//        printf("\nVariables after interpretation:\n");
//        MemoryFunctions.printRegister(m, Numerical);
//        printf("\n");
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

    MEMORY_NEW_NUM(m, Int, "s", 5);
    MEMORY_NEW_STR(m, "St", "H3110_WR1D");

    printf("Variables before interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    printf("\n");

    Expression *e = interpretFile(&m, &files[0]);

    printf("Variables after interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    MemoryFunctions.printRegister(&m, String);
    printf("\n");

    return EXIT_SUCCESS;
}