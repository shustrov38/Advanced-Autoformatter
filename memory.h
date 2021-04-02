#ifndef ADVANCED_AUTOFORMATTER_MEMORY_H
#define ADVANCED_AUTOFORMATTER_MEMORY_H

#include "libraries.h"

#define ARRAY_LEN 100

#define MAX_STRING_LEN 20
#define VARIABLES_COUNT 50
#define FUNCTIONS_COUNT 50
#define STRUCTS_COUNT 50

typedef enum {
    Int,
    Double,
    String
} VarType;

typedef struct variant_t Variant;

struct variant_t {
    union {
        int i;
        double d;
        char *s;
    };
    VarType type;
};

typedef struct register_t Register;

struct register_t {
    int total;
    Variant *items;
    char **names;
};

#define REGISTERS_COUNT 10

#define INIT_MEMORY(MEM) Memory MEM; \
InitMemory(&MEM)

typedef struct memory_t Memory;

struct memory_t {
    Register registers[REGISTERS_COUNT];
    size_t type[REGISTERS_COUNT];
    int total;

    // returns pointer to register by variable type
    Register *(*getRegister)(Memory *, VarType);

    // adds new variable to register
    void (*new)(Memory *, VarType, char *, Variant);

    // prints register
    void (*printRegister)(Memory *, VarType);

    // returns pointer to value of variable by name
    Variant *(*getValue)(Memory *, char *);
};

#define MEMORY_NEW(MEMORY, TYPE, NAME, VALUE) { \
Variant t = {VALUE, TYPE};                      \
MEMORY.new(&MEMORY, TYPE, NAME, t);             \
}

void InitMemory(Memory *m);

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
