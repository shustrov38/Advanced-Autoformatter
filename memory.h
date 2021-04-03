#ifndef ADVANCED_AUTOFORMATTER_MEMORY_H
#define ADVANCED_AUTOFORMATTER_MEMORY_H

#include "libraries.h"

#define ARRAY_LEN 100

#define MAX_STRING_LEN 20
#define VARIABLES_COUNT 50
#define FUNCTIONS_COUNT 50
#define STRUCTS_COUNT 50

typedef enum {
    Numerical,
    String
} VarType;

typedef enum {
    Int,
    Double,
    Unsigned
} NumType;

typedef struct variant_t Variant;

struct variant_t {
    double d;
    char *s;

    int isOverflowed;
    NumType numCap;
    VarType type;
};

typedef struct register_t Register;

struct register_t {
    int total;
    Variant *items;
    char **names;
};

#define REGISTERS_COUNT 10

typedef struct memory_t Memory;

struct memory_t {
    Register registers[REGISTERS_COUNT];
    VarType type[REGISTERS_COUNT];
    int total;
};

struct memory_functions_t {
    // inits memory
    void (*init)(Memory *);

    // returns pointer to register by variable type
    Register *(*getRegister)(Memory *, VarType);

    // adds new variable to register
    void (*newNum)(Memory *, NumType, char *, Variant);

    // prints register
    void (*printRegister)(Memory *, VarType);

    // returns pointer to value of variable by name
    Variant *(*getValue)(Memory *, char *);

    // increment
    void (*inc)(Memory *, char *);

    // decrement
    void (*dec)(Memory *, char *);

    void (*overFlowChecker)(Memory *);
};

extern struct memory_functions_t MemoryFunctions;

#define INIT_MEMORY(MEM) Memory MEM; \
MemoryFunctions.init(&MEM)

#define MEMORY_NEW(MEMORY, TYPE, NAME, VALUE) { \
Variant t = {VALUE, TYPE};                      \
MemoryFunctions.newNum(&MEMORY, TYPE, NAME, t);    \
}

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
