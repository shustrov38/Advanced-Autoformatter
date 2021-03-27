#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma region FileUtilities

#define MAX_FILES 10

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

#pragma region FunctionUtilities

#define MAX_STRING_LEN 10
#define FUNCTIONS_COUNT 50

struct functions_t {
    int size;
    int used[FUNCTIONS_COUNT];
    char name[MAX_STRING_LEN][FUNCTIONS_COUNT];
} Functions;

void collectFunctions(FILE *files[MAX_FILES], unsigned count) {
    for (int i = 0; i < count; ++i) {
        fseek(files[i], 0, SEEK_SET);

    }
}

#pragma endregion FunctionUtilities

int main(const int argc, const char *argv[]) {
    if (argc == 1) {
        printf("No filenames specified.\n");
        return EXIT_FAILURE;
    }

    FILE *headers[MAX_FILES], *sources[MAX_FILES];
    unsigned headersCount = 0, sourcesCount = 0;

    for (int i = 1; i < argc; ++i) {
        switch (isCorrectFilename(argv[i])) {
            case Header:
                headers[headersCount++] = (FILE *) fopen(argv[i], "rw");
                break;
            case Source:
                sources[sourcesCount++] = (FILE *) fopen(argv[i], "rw");
                break;
            default:
                printf("File \"%s\" has incorrect extension.\n", argv[i]);
                break;
        }
        if (headers[headersCount - 1] == NULL || sources[sourcesCount - 1] == NULL) {
            printf("File \"%s\" doesnt exists\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // TODO: collect all functions from files

    for (int i = 0; i < headersCount; ++i) {
        fclose(headers[i]);
    }

    for (int i = 0; i < sourcesCount; ++i) {
        fclose(sources[i]);
    }

    return EXIT_SUCCESS;
}
