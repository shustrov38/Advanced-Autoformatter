#include "fileUtils.h"

typedef enum {
    Incorrect,
    Header,
    Source
} FileExtension;

int getLineLength(char **line) {
    int size = -1;
    while (line[++size][0]);
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
        int i = 0;
        while (strcmp(line[i], "(") != 0 && i < length) ++i;

        if (i == length) {
            // its variable definition
            continue;
        } else if (i) {
            strcpy(file->functions[file->funcCount].name, line[i - 1]);
            // TODO: store all inputs for current function
            //  etc names and types.
        }

        // store function body
        while (strcmp(file->code->codeLines[++k][0], "}") != 0) {
            line = file->code->codeLines[k];
            length = getLineLength(line);

            // find nested
            if (length > 1) {
                // TODO: check if line[0] not for, switch, if and etc..
                strcpy(file->functions[file->funcCount].nestedFunctions[file->functions[file->funcCount].nestedCount++], line[0]);
            } else if (length > 2) {
                strcpy(file->functions[file->funcCount].nestedFunctions[file->functions[file->funcCount].nestedCount++], line[1]);
            }

            // copy loop
            assert(file->functions[file->funcCount].code != NULL);
            int lastLine = file->functions[file->funcCount].code->linesCnt;
            for (i = 0; i < length; ++i) {
                strcpy(file->functions[file->funcCount].code->codeLines[lastLine][i], line[i]);
            }
            file->functions[file->funcCount].code->linesCnt++;
        }

        file->funcCount++;
    }
}

void printAllFunctions(FileData *file) {
    for (int i = 0; i < file->funcCount; ++i) {
        printf("function: [%s]\n", file->functions[i].name);
        printf("------------------------------------------------------\n");
        printCode(file->functions[i].code);
        printf("------------------------------------------------------\n");
        printf("\n");
    }
}