#include "functions.h"

Function *createFunctions() {
    Function *f = malloc(ARRAY_LEN * sizeof(Function));
    for (int i = 0; i < ARRAY_LEN; ++i) {
        memset(f[i].name, 0, MAX_STRING_LEN);

        f[i].inputsCount = 0;
        for (int j = 0; j < ARRAY_LEN; ++j) {
            memset(f[i].nestedFunctions[j], 0, MAX_STRING_LEN);
        }

        f[i].code = createCodeLineStruct();

        f[i].nestedCount = 0;
        for (int j = 0; j < ARRAY_LEN; ++j) {
            memset(f[i].nestedFunctions[j], 0, MAX_STRING_LEN);
        }
    }
    return f;
}