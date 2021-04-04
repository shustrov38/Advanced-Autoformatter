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
} VariableType;

typedef enum {
    Int,
    Double,
    Unsigned
} NumericType;

typedef struct variant_t Variant;

struct variant_t {
    // parameters for numerical variable
    int isOverflowed;
    NumericType numType;
    double d;

    // string variable
    char *s;

    // just variable type
    VariableType varType;
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
    VariableType type[REGISTERS_COUNT];
    int total;
};


// TODO: add possibility to create new String variables.
struct memory_functions_t {
    // inits memory
    void (*init)(Memory *);

    // returns pointer to register by variable varType
    Register *(*getRegister)(Memory *, VariableType);

    // adds new variable to register
    void (*newNum)(Memory *, char *, Variant);

    // prints register
    void (*printRegister)(Memory *, VariableType);

    // returns pointer to value of variable by name
    Variant *(*getValue)(Memory *, char *);

    // increment
    void (*inc)(Memory *, char *);

    // decrement
    void (*dec)(Memory *, char *);

    // check if unsigned variables overflowed
    void (*overflowCheck)(Memory *);
};

extern struct memory_functions_t MemoryFunctions;

#define INIT_MEMORY(MEM) Memory MEM; \
MemoryFunctions.init(&MEM)

// TODO: add #define for newStr function that you will include.

#define MEMORY_NEW_NUM(MEMORY, TYPE, NAME, VALUE) { \
Variant t = {.d = VALUE,.isOverflowed = 0, .numType = TYPE, .varType = Numerical}; \
MemoryFunctions.newNum(&MEMORY, NAME, t); \
}

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
