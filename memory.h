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
};

#define MEMORY_NEW(MEMORY, TYPE, NAME, VALUE) { \
Variant t = {VALUE, TYPE};                      \
new(&MEMORY, TYPE, NAME, t);                    \
}

void InitMemory(Memory *m);

Register *getRegister(Memory *m, VarType type);

void new(Memory *m, VarType type, char *name, Variant item);

void printRegister(Memory *m, VarType type);

Variant *getValue(Memory *m, char *name);

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
