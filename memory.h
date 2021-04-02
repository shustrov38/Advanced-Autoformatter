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

#define INIT_REGISTER(REG) Register REG; \
InitRegister(&REG)

typedef struct register_t Register;

struct register_t {
    int total;
    Variant *items;
    char **names;
};

void InitRegister(Register *r);

#define REGISTERS_COUNT 10

#define INIT_MEMORY(MEM) Memory MEM; \
InitMemory(&MEM)

typedef struct memory_t Memory;

struct memory_t {
    Register registers[REGISTERS_COUNT];
    size_t size[REGISTERS_COUNT];
    int total;

    Register *(*getRegister)(Memory *, size_t);

    void (*new)(Memory *, size_t, char *, Variant);

    void (*printRegister)(Memory *, size_t);
};

void InitMemory(Memory *m);

Register *getRegister(Memory *m, size_t size);

void new(Memory *m, size_t size, char *name, Variant item);

void printRegister(Memory *m, size_t size);

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
