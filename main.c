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
#define CRITICAL_EXE_ST 12
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
    Variant nTmp = {.varType = None, .isInited = 0, .d = 0};
    MemoryFunctions.newNum(m, "?nTmp", nTmp);
    Variant constZero = {.varType = Numerical, .isInited = 1, .d = 0};
    MemoryFunctions.newNum(m, "0", nTmp);
    int inMain = 0;
    int executionPathCorruption = 0;
    for (int i = 0; i < file->code->linesCnt; ++i) {
//        if (!isArithmeticCodeLine()) continue;

        // length of one code line, includes ';'
        int codeLineLength = getLineLength(file->code->codeLines[i]);

        // add and convert expression from code line to calculus expression
        int q = addExpression(e, size, file->code->codeLines[i], codeLineLength, meta, i + 1, &exeSt, &reqSize, bools,
                              &bcnt);
        size += q;
    }
    size++;

    // create filename for listing
    char listingFileName[100];
    memset(listingFileName, 0, 50);
    strcat(listingFileName, "../listings/LST_");
    strcat(listingFileName, strrchr(file->filename, '/') + 1);
    strcat(listingFileName, ".txt");
//    printf("%s\n", listingFileName);
    FILE *listingFile = fopen(listingFileName, "w");

    // iterate through Expressions and interpret each of them
    for (int i = 0; i < 1000; ++i) {
        if (executionPathCorruption == CRITICAL_EXE_ST){
            printf("Interpretation of %s was stopped to avoid memory corruptions and overflowes:"
                   "\nto many uneven execution paths [%d] were found",file->filename,CRITICAL_EXE_ST);
            return e;
        }
        if(!strcmp(e[i].code[0], "int") && !strcmp(e[i].code[1], "main")) inMain = 1;
        rpnProcessor *outStack = rpnProcInit();

        fprintf(listingFile, "\n");
        for (int z = 0; z < e[i].size; z++) {
            fprintf(listingFile, "%s ", e[i].code[z]);
        }

        //GOTO & tags logic
        if (!strcmp(e[i].code[0], "endof") && strncmp(e[i].code[1], "?if", 3) != 0) {
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            int u = 0;
            for (; u < bcnt; u++) {
                if (!strcmp(bools[u].name, e[i].code[1])) break;
            }
            for (int ff = 0; ff < bools[u].itCnt; ff++) {
                double tmp;
                if (MemoryFunctions.getValue(m, bools[u].expr[ff]) == NULL) tmp = 0;
                else tmp = MemoryFunctions.getValue(m, bools[u].expr[ff])->d;
                if (bools[u].iVals[ff] != tmp) {
                    bools[u].nonConstIter = 1;
                    break;
                }
            }
            if(strncmp(e[i].code[1],"?while",6)==0 ||strncmp(e[i].code[1],"?for",4)==0){
            bools[u].state = (bools[u].builtInIter || bools[u].nonConstIter) &&
                             (bools[u].hasNoUnevenExecutionPath || bools[u].builtInIter) ||
                             bools[u].fullInit && bools[u].itCnt && bools[u].nonConstIter || bools[u].isBreak;
            if (!bools[u].state) {
                if (bools[u].isBroken == 0)printf("Line %d: Uneven execution conditions may lead to endless loop.\n", bools[u].line);
#ifdef __INTERPRET_DEBUG__
                printf("\n\n");
                for (int y = 0; y < bcnt; y++){
                    printf(" %s\tin line %d may be stopped via break: %d  fully inited: %d has nonconstant iters: %d\n",
                           bools[y].name, bools[y].line, bools[y].isBreak, bools[y].fullInit, bools[y].nonConstIter);
                }
                printf("\n\n");
#endif
                int executionLineNum = i;
                while (!(!strcmp(e[executionLineNum].code[0], "endof") &&
                         !strcmp(e[executionLineNum].code[1], e[i].code[1]))) {
                    executionLineNum++;
                }
                MEMORY_NEW_NUM(*m, Int, e[i].code[1], 0);
                i = executionLineNum+1;
                executionPathCorruption++;
                bools[u].isBroken = 1;
                continue;
            }}


            if (MemoryFunctions.getValue(m, e[i].code[1])->d > 0 && strncmp(e[i].code[1], "?dwhl", 5) != 0) {
                int executionLineNum = i - 1;

                while (strcmp(e[executionLineNum].code[0], e[i].code[1])) {
                    executionLineNum--;
                }
                i = executionLineNum - 1;
                continue;
            } else if (MemoryFunctions.getValue(m, e[i].code[1])->d > 0 && strncmp(e[i].code[1], "?dwhl", 5) == 0) {
                int executionLineNum = i - 1;
                while (strcmp(e[executionLineNum].code[1], e[i].code[1]) != 0 &&
                       strcmp(e[executionLineNum].code[0], "begin") != 0) {
                    executionLineNum--;
                }
                i = executionLineNum;
                continue;
            } else {
                continue;
            }

        } else if (!strcmp(e[i].code[0], "begin") && strncmp(e[i].code[1], "?dwhl", 5) != 0) {
//            printf(" == %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
            int u = 0;
            for (; u < bcnt; u++) {
                if (!strcmp(bools[u].name, e[i].code[1])) break;
            }
            bools[u].state = 1;
            for (int ff = 0; ff < bools[u].itCnt; ff++) {
                double tmp;
                if ((__getOpID(bools[u].expr[ff]) == VAR) &&
                    (MemoryFunctions.getValue(m, bools[u].expr[ff])->varType == None
                     || MemoryFunctions.getValue(m, bools[u].expr[ff])->isInited == 0)) {
                    if(inMain == 1)bools[u].state = 0;
                    tmp = 0;
                }
                if (MemoryFunctions.getValue(m, bools[u].expr[ff]) == NULL) tmp = 0;
                else tmp = MemoryFunctions.getValue(m, bools[u].expr[ff])->d;
                bools[u].iVals[ff] = tmp;
            }
            if (strncmp(e[i].code[1], "?if", 3)==0){
                bools[u].state = bools[u].itCnt;
                if (!bools[u].state) {
                    bools[u].state = 1;
                    printf("Line %d: If expression defined by a const.\n", bools[u].line);
#ifdef __INTERPRET_DEBUG__
                    printf("\n\n");
                for (int y = 0; y < bcnt; y++){
                    printf(" %s\tin line %d may be stopped via break: %d  fully inited: %d has nonconstant iters: %d\n",
                           bools[y].name, bools[y].line, bools[y].isBreak, bools[y].fullInit, bools[y].nonConstIter);
                }
                printf("\n\n");
#endif
                }
            }
            else if (!bools[u].state) {
                if(bools[u].isBroken == 0)printf("Line %d: Uneven execution conditions may lead to endless loop.\n", bools[u].line);
#ifdef __INTERPRET_DEBUG__
                printf("\n\n");
                for (int y = 0; y < bcnt; y++){
                    printf(" %s\tin line %d may be stopped via break: %d  fully inited: %d has nonconstant iters: %d\n",
                           bools[y].name, bools[y].line, bools[y].isBreak, bools[y].fullInit, bools[y].nonConstIter);
                }
                printf("\n\n");
#endif
                int executionLineNum = i;
                while (!(!strcmp(e[executionLineNum].code[0], "endof") &&
                         !strcmp(e[executionLineNum].code[1], e[i].code[1]))) {
                    executionLineNum++;
                }
                MEMORY_NEW_NUM(*m, Int, e[i].code[1], 0);
                executionPathCorruption++;
                bools[u].isBroken = 1;
                i = executionLineNum+1;
                continue;
            }

            fprintf(listingFile, "== %f", MemoryFunctions.getValue(m, e[i].code[1])->d);
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

        } else if (!strcmp(e[i].code[0], "stop")) {
            MEMORY_NEW_NUM(*m, Int, e[i].code[1], 0);
            int executionLineNum = i - 1;
            while (!(strcmp(e[executionLineNum].code[0], "begin") == 0 &&
                     strcmp(e[executionLineNum].code[1], e[i].code[1]) == 0)) {
                executionLineNum--;
            }
            i = executionLineNum - 1;
            continue;
        } else if (!strcmp(e[i].code[0], "skip") && !strncmp(e[i].code[1], "?for", 4)) {
            int executionLineNum = i;
            while (!(!strcmp(e[executionLineNum].code[0], "endof") &&
                     !strcmp(e[executionLineNum].code[1], e[i].code[1]))) {
                executionLineNum++;
            }
            if (!strncmp(e[i].code[1], "?for", 4)) i = executionLineNum - 2;
            continue;
        } else if (!strcmp(e[i].code[0], "skip") &&
                   (!strncmp(e[i].code[1], "?while", 6) || !strncmp(e[i].code[1], "?dwhl", 5))) {
            int executionLineNum = i;
            while (!(!strcmp(e[executionLineNum].code[0], "endof") &&
                     !strcmp(e[executionLineNum].code[1], e[i].code[1]))) {
                executionLineNum++;
            }
            i = executionLineNum - 1;
            continue;
        }

        //INIT logics
        if (!strcmp(e[i].code[0], "int")) {
            MEMORY_NEW_NUM(*m, Int, e[i].code[1], 0);
        } else if (!strcmp(e[i].code[0], "double")) {
            MEMORY_NEW_NUM(*m, Double, e[i].code[1], 0);
        } else if (!strcmp(e[i].code[0], "unsigned")) {
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

    fclose(listingFile);

#ifdef __INTERPRET_DEBUG__
    printf("\n\n");
    for (int y = 0; y < bcnt; y++){
        printf(" %s\tin line %d may be stopped via break: %d  fully inited: %d has nonconstant iters: %d\n",
                    bools[y].name, bools[y].line, bools[y].isBreak, bools[y].fullInit, bools[y].nonConstIter);
    }
    printf("\n\n");
#endif

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

    printFunctionsCallTable(files, filesCount); // data about functions and nested cycles
    checkIncludeCycles(files, filesCount);

#ifdef WORK_WITH_MEMORY


//    printf("Variables before interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    printf("\n");

    for (int i = 0; i < filesCount; ++i) {
        INIT_MEMORY(m);
        Expression *e = interpretFile(&m, &files[i]);
    }
//
//    printf("Variables after interpretation:\n");
//    MemoryFunctions.printRegister(&m, Numerical);
//    printf("\n");
#endif

    return EXIT_SUCCESS;
}
