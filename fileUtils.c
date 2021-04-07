#include "fileUtils.h"

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

        strcpy(files[i - 1].filename, argv[i]);

        // prepare code array for using
        for (int j = 0; j < MAX_CODE_LEN; ++j) {
            memset(code[j], 0, MAX_DIVISOR_LEN);
        }

        int n = splitSyntax(files[i - 1].filename, code);

#ifdef __PARSER_OUT_DEBUG__
        for (int j = 0; j < ARRAY_LEN; ++j) {
            initFunction(&files[i - 1].functions[j]);
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

    int i = 0;
    while (i < length && line[i][0] != '(') ++i;

    if (i == length || i == 0) {
        return -1;
    }

    int isBlock = !strcmp(line[i - 1], "for") ||
                  !strcmp(line[i - 1], "switch") ||
                  !strcmp(line[i - 1], "while") ||
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
        while (1) {
            line = file->code->codeLines[++k];
            length = getLineLength(line);

            for (int i = 0; i < length; ++i) {
                if (!strcmp(line[i], "{")) balance++;
                if (!strcmp(line[i], "}")) balance--;
            }

            if (balance == 0) {
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

int findFunction(vector *names, char *key) {
    for (int i = 0; i < Vec.size(names); ++i) {
        if (!strcmp((char *) Vec.get(names, i), key)) {
            return i;
        }
    }
    return -1;
}

void printFunctionsCallTable(FileData *files, int size) {
    INIT_VECTOR(names);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < files[i].funcCount; ++j) {
            Vec.push(&names, files[i].functions[j].name);
        }
    }

    int table[Vec.size(&names)][Vec.size(&names)];

    for (int i = 0; i < Vec.size(&names); ++i) {
        for (int j = 0; j < Vec.size(&names); ++j) {
            table[i][j] = 0;
        }
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < files[i].funcCount; ++j) {
//            printf("%s: ", files[i].functions[j].name);
            int fromIndex = findFunction(&names, files[i].functions[j].name);
            for (int k = 0; k < files[i].functions[j].nestedCount; ++k) {
//                printf(" %s", files[i].functions[j].nestedFunctions[k]);
                int toIndex = findFunction(&names, files[i].functions[j].nestedFunctions[k]);
                table[fromIndex][toIndex] = 1;
            }
//            printf("\n");
        }
    }

    int recCounter = 0;

    printf("Call table for functions:\n\n");
    for (int i = 0; i < Vec.size(&names); ++i) {
        int k = 0;
        for (int j = 0; j < Vec.size(&names); ++j) {
            k += table[i][j];
        }
        printf("%s() : ", (char *) Vec.get(&names, i));
        if (k) {
            printf("{");
            for (int j = 0; j < Vec.size(&names); ++j) {
                if (table[i][j]) {
                    if (i == j) recCounter++;
                    printf(" %s()", (char *) Vec.get(&names, j));
                }
            }
            printf(" }\n");
        } else {
            printf("have no sub calls\n");
        }
    }

    printf("\n");

    if (recCounter) {
        printf("Recursive functions:\n\n");
        for (int i = 0; i < Vec.size(&names); ++i) {
            if (table[i][i]) {
                printf("%s()\n", (char *) Vec.get(&names, i));
            }
        }
    } else {
        printf("Have no recursive functions.\n");
    }
}
