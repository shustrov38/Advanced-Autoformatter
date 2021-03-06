#include "fileUtils.h"
#include "cycleFounder.h"

typedef enum {
    Incorrect,
    Header,
    Source
} FileExtension;

int getLineLength(char **line) {
    if (line == NULL) return 0;
    int size = 0;
    while (line[size] != NULL && line[size][0]) ++size;
    return size;
}

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

        files[i - 1].isHeader = (ext == Header);

        strcpy(files[i - 1].filename, argv[i]);

        // prepare code array for using
        for (int j = 0; j < MAX_CODE_LEN; ++j) {
            memset(code[j], 0, MAX_DIVISOR_LEN);
        }

        int n = splitSyntax(files[i - 1].filename, code);

#ifdef __PARSER_OUT_DEBUG__
        for(int j = 0;j < n;j++) {
            printf("[%s]\n", code[j] );
        }
        printf("\n");
#endif

        files[i - 1].code = createCodeLineStruct();
        splitLines(files[i - 1].code, n, code);

        files[i - 1].functions = createFunctions();
        files[i - 1].funcCount = 0;
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

int isFunctionCall(char **line) {
    int length = getLineLength(line);

    for (int i = 0; i < length; ++i) {
        if (!strcmp(line[i], "//")) {
            length = i + 1;
            break;
        }
    }

    int i = 0;
    while (i < length && line[i][0] != '(') ++i;

    if (i == length || i == 0) {
        return -1;
    }

    int isBlock = !strcmp(line[i - 1], "for") ||
                  !strcmp(line[i - 1], "switch") ||
                  !strcmp(line[i - 1], "while") ||
                  !strcmp(line[i - 1], "do") ||
                  !strcmp(line[i - 1], "if");

    if (!isBlock) {
        return i - 1;
    } else {
        return -1;
    }
}

void loadFunctions(FileData *file) {
    // TODO: find way to find function body by prototype or header file.
    for (int k = 0; k < file->code->linesCnt; ++k) {
        char **line = file->code->codeLines[k];
        int length = getLineLength(line);

        // TODO: find way to move type checking into function.
        if (length > 1 && !strcmp(line[0], "char") && !strcmp(line[1], "*")) {
            file->functions[file->funcCount].output.varType = String;
        } else {
            file->functions[file->funcCount].output.varType = Numerical;
            int isUnsigned = !strcmp(line[1], "unsigned");

            int isDouble = !strcmp(line[0], "float") ||
                           !strcmp(line[0], "double") ||
                           length > 1 && !strcmp(line[0], "long") && !strcmp(line[1], "double");

            if (isUnsigned) {
                file->functions[file->funcCount].output.numType = Unsigned;
            } else if (isDouble) {
                file->functions[file->funcCount].output.numType = Double;
            } else {
                file->functions[file->funcCount].output.numType = Int;
            }
        }

        // check if its function

        int funcIndex;
        if ((funcIndex = isFunctionCall(line)) == -1) {
            // its variable definition
            continue;
        } else {
            strcpy(file->functions[file->funcCount].name, line[funcIndex]);
            // TODO: store all inputs for current function
            //  etc names and types.
        }

        // store function body

        int balance = 1;
        int linesCnt = 0;
        while (1) {
            linesCnt++;
            line = file->code->codeLines[++k];
            length = getLineLength(line);

            for (int i = 0; i < length; ++i) {
                if (!strcmp(line[i], "{")) balance++;
                if (!strcmp(line[i], "}")) balance--;
            }

            if (balance == 0) {
                file->functions[file->funcCount].code->linesCnt = linesCnt - 1;
                break;
            }

            // find nested
            int nestedIndex;
            if ((nestedIndex = isFunctionCall(line)) != -1) {
                strcpy(file->functions[file->funcCount].nestedFunctions[file->functions[file->funcCount].nestedCount++],
                       line[nestedIndex]);
            }

            // copy loop
            file->functions[file->funcCount].code->codeLines[file->functions[file->funcCount].code->linesCnt++] = line;
        }

        file->funcCount++;
    }
}

void printAllFunctions(FileData *file) {
    for (int i = 0; i < file->funcCount; ++i) {
        printf("function: [%s]\n", file->functions[i].name);
        printf("------------------------------------------------------\n");
        printCode(file->functions[i].code);
        printf("----------------\nnested:\n");
        for (int j = 0; j < file->functions[i].nestedCount; ++j) {
            printf("%s\n", file->functions[i].nestedFunctions[j]);
        }
        printf("------------------------------------------------------\n");
        printf("\n");
    }
}

int findFunctionInVectorOfNames(vector *names, char *key) {
    for (int i = 0; i < Vec.size(names); ++i) {
        if (!strcmp((char *) Vec.get(names, i), key)) {
            return i;
        }
    }
    return -1;
}

// find function index in all files
int findFunction(FileData *files, int size, char *key, int *file_index, int *func_index) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < files[i].funcCount; ++j) {
            if (!strcmp(files[i].functions[j].name, key)) {
                *file_index = i;
                *func_index = j;
                return 1;
            }
        }
    }

    return 0;
}


#define func files[file_index].functions[func_index]

// go through all functions in all the files, calculate summary cycle depth (including functions sub calls)
void calculateNestedCyclesDepth(FileData *files, int size, int file_index, int func_index, int *depth) {
    int balance = 0;
    int cycleCount = 0;

    int _size = 0;
    int lastCycleBalance[250];

    int addDepth = 0, locDepth = 0;

    int n = func.code->linesCnt;

    char **line;
    int length;
    for (int i = 0; i < n; ++i) {
        line = func.code->codeLines[i];
        length = getLineLength(line);

        int funcIndex;
        if ((funcIndex = isFunctionCall(line)) == -1) {
            // its variable definition (maybe cycle / if)
        } else {
            // its function call
            int file_i = 0, func_j = 0;
            if (findFunction(files, size, line[funcIndex], &file_i, &func_j)) {
                locDepth = 0;
                calculateNestedCyclesDepth(files, size, file_i, func_j, &locDepth);
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("locDepth: %d\n", locDepth);
#endif //__CYCLE_DETECTOR_DEBUG__
                addDepth = __max(cycleCount + locDepth, addDepth);
            }
        }

#ifdef __CYCLE_DETECTOR_DEBUG__
        for (int j = 0; j < length; ++j) {
            printf("%s ", line[j]);
        }
        printf("\n");
#endif //__CYCLE_DETECTOR_DEBUG__

        // go through code line and check cycles
        for (int j = 0; j < length; ++j) {
            if (!strcmp(line[j], "for") || !strcmp(line[j], "do") || !strcmp(line[j], "while")) {
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("plus for, %d\n", balance);
#endif // __CYCLE_DETECTOR_DEBUG__
                cycleCount++;
                addDepth = __max(cycleCount, addDepth);
                lastCycleBalance[_size++] = balance;
                balance++;
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("open, %d\n", balance);
#endif // __CYCLE_DETECTOR_DEBUG__
                break;
            }

            if (!strcmp(line[j], "{")) {
                balance++;
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("open, %d\n", balance);
#endif // __CYCLE_DETECTOR_DEBUG__
            }

            if (!strcmp(line[j], "}")) {
                balance--;
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("close, %d\n", balance);
#endif // __CYCLE_DETECTOR_DEBUG__
            }

            if (_size > 0 && balance == lastCycleBalance[_size - 1]) {
#ifdef __CYCLE_DETECTOR_DEBUG__
                printf("minus for, %d\n", balance);
#endif // __CYCLE_DETECTOR_DEBUG__
                cycleCount--;
                _size--;
            }
        }
    }

    *depth += addDepth;
}

#undef func

// prints all data about functions (call table, true recursive, cycle depth)
void printFunctionsCallTable(FileData *files, int size) {
    INIT_VECTOR(names);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < files[i].funcCount; ++j) {
            Vec.push(&names, files[i].functions[j].name);
        }
    }

    int n = Vec.size(&names);
    int **table = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        table[i] = (int *) malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            table[i][j] = 0;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < files[i].funcCount; ++j) {
            int fromIndex = findFunctionInVectorOfNames(&names, files[i].functions[j].name);
            for (int k = 0; k < files[i].functions[j].nestedCount; ++k) {
                int toIndex = findFunctionInVectorOfNames(&names, files[i].functions[j].nestedFunctions[k]);
                table[fromIndex][toIndex] = 1;
            }
        }
    }

    int recCounter = 0;

    printf("Call table for functions:\n");
    for (int i = 0; i < n; ++i) {
        int k = 0;
        for (int j = 0; j < n; ++j) {
            k += table[i][j];
        }
        printf("%s() : ", (char *) Vec.get(&names, i));
        if (k) {
            printf("{");
            for (int j = 0; j < n; ++j) {
                if (table[i][j]) {
                    if (i == j) {
                        recCounter++;
                    }
                    printf(" %s()", (char *) Vec.get(&names, j));
                }
            }
            printf(" }\n");
        } else {
            printf("have no sub calls\n");
        }
    }

    printf("\n");

    printf("True recursive functions:\n");
    if (recCounter) {
        for (int i = 0; i < n; ++i) {
            if (table[i][i]) {
                printf("%s()\n", (char *) Vec.get(&names, i));
            }
        }
    } else {
        printf("Program has not got any recursive functions.\n");
    }

    printf("\n");

    printf("Maximum depth of nested cycle:\n");
    if (findCycle(&names, table, n, NULL, NULL)) {
        printf("Program has cycle of nested functions (maybe recursive), so the depth may be infinite.\n");
    } else {
        // calc cycle length

        int maxDepth = 0;
        char *name = "main";

        // check all functions as start points
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < files[i].funcCount; ++j) {
                int depth = 0;
                calculateNestedCyclesDepth(files, size, i, j, &depth);
                if (depth > maxDepth) {
                    name = files[i].functions[j].name;
                }
                maxDepth = __max(depth, maxDepth);
            }
        }

        printf("Founded nested cycle with depth %d from function %s(). Real depth of cycle may be different.\n",
               maxDepth, name);
    }
}

// TODO: ...
void checkIncludeCycles(FileData *files, int size) {
    INIT_VECTOR(names);
    for (int i = 0; i < size; ++i) {
        if (files[i].isHeader) {
            Vec.push(&names, files[i].filename);
        }
    }

    int n = Vec.size(&names);;
    int **table = (int **) malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        table[i] = (int *) malloc(n * sizeof(int));
    }

    // parse all dependencies for all header files

    for (int i = 0; i < size; ++i) {
        if (files[i].isHeader) {
            for (int j = 0; j < files[i].code->linesCnt; ++j) {
                char **line = files[i].code->codeLines[j];
                int length = getLineLength(line);

                if (!strcmp(line[0], "#include") && !strcmp(line[1], "\"")) {
                    printf("%s\n", line[0]);
                }
            }
        }
    }
}