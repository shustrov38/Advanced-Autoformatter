#include "libraries.h"
#include "Autoformatter/parser.h"
#include "Autoformatter/lineMaker.h"
#include "Autoformatter/outputFormat.h"
#include "Autoformatter/nameChecking.h"

#include "Calculator/ops.h"
#include "Calculator/RPN.h"
#include "Calculator/stack.h"
#include "Calculator/tree.h"
#include "Calculator/parser.h"

#include "vector.h"
#include "memory.h"

#include "fileUtils.h"

#define WORK_WITH_MEMORY

Expression *interpretFile(Memory *m, FileData *file) {
    // size of the Expression array
    int size = 0;
    Expression *e = createExpressions();
    Stack *meta = stCreate();
    INIT_VECTOR(exeSt);
    INIT_VECTOR(reqSize);
    bool *bools = (bool *) malloc(100 * sizeof(bool));
    int bcnt = 0;
    // TODO: it might be better to start the interpretation directly from the main function, ?????

    // TODO: change cycle, choose only arithmetic lines of code.
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        int q = addExpression(e, size, file->code->codeLines[i], codeLineLength, meta, i+1, &exeSt, &reqSize, bools, &bcnt);
        printf("%d", bcnt);
        size+=q;
    }
    size++;

    // iterate through Expressions and interpret each of them
    for (int i = 0; i < size; ++i) {
        rpnProcessor *outStack = rpnProcInit();

        printf("\n");
        for (int z = 0; z < e[i].size; z++) {
            printf(" %s", e[i].code[z]);
        }
        //GOTO & tags logic
        //TODO: CONTINE & BREAK tags logics + real. in parcer.c


        if (!strcmp(e[i].code[0], "endof") && !(e[i].code[1][1] == 'i' && e[i].code[1][2] == 'f')) { //TODO: make a NORMAL notIF condition + break && continue
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            if (MemoryFunctions.getValue(m, e[i].code[1])->d > 0) {
                int executionLineNum = i;
                while (strcmp(e[executionLineNum].code[0], e[i].code[1])) {
                    executionLineNum--;
                }
                i = executionLineNum - 1;
                continue;
            } else {
                continue;
            }

        } else if (!strcmp(e[i].code[0], "begin") && !(e[i].code[1][1] == 'i' && e[i].code[1][2] == 'f')) {
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            if (MemoryFunctions.getValue(m, e[i].code[1])->d == 0) {
                int executionLineNum = i;
                while (!(!strcmp(e[executionLineNum].code[0], "endof") &&
                         !strcmp(e[executionLineNum].code[1], e[i].code[1]))) {
                    executionLineNum++;
                }
                i = executionLineNum - 1;
                continue;
            } else {
                continue;
            }

        } else if (!strcmp(e[i].code[0],"begin") && (e[i].code[1][1] == 'i' && e[i].code[1][2] == 'f')){
            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            if(MemoryFunctions.getValue(m, e[i].code[1])->d == 0){
                int executionLineNum = i;
                while(!(!strcmp(e[executionLineNum].code[0],"endof") && !strcmp(e[executionLineNum].code[1],e[i].code[1]))){
                    executionLineNum++;
                }
                i = executionLineNum-1;
                continue;
            }

            else{
                continue;
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

        //FOR validity checker
        //TODO: (1)-E A 3 cond (2)- E diff i (3)-E break
        // (1) --> parcer.c;



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
    printf("\n");
    return e;
}

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FileData files[10];
    int filesCount = loadFiles(files, argc, argv);
//    printAllFiles(files, filesCount); // files source code

    for (int i = 0; i < filesCount; ++i) {
        outputFiles(files[i].filename, files[i].code);
    }

    for (int i = 0; i < filesCount; ++i) {
        checkNames(files[i].filename, files[i].code);
    }

    for (int i = 0; i < filesCount; ++i) {
        loadFunctions(&files[i]);
//        printAllFunctions(&files[0]);
    }

//    printFunctionsCallTable(files, filesCount); // data about functions and nested cycles
//    checkIncludeCycles(files, filesCount); // need work

#ifdef WORK_WITH_MEMORY
    INIT_MEMORY(m);

    MEMORY_NEW_NUM(m, Int, "s", 5);
    MEMORY_NEW_STR(m, "St", "H3110_WR1D");

    printf("Variables before interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    printf("\n");

    Expression *e = interpretFile(&m, &files[0]);

    printf("Variables after interpretation:\n");
    MemoryFunctions.printRegister(&m, Numerical);
    printf("\n");
#endif

    return EXIT_SUCCESS;
}