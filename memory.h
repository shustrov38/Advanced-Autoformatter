#ifndef ADVANCED_AUTOFORMATTER_MEMORY_H
#define ADVANCED_AUTOFORMATTER_MEMORY_H

#include "libraries.h"

typedef enum {
    Numerical,
    String,
    None
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
    int isInited;
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

typedef enum {
    For,
    If,
    While
} boolean_type;

struct boolean_e_c {
    char            *name;
    char            **expr;
    double          *iVals;
    int             itCnt;
    boolean_type    type;
    int             line;
    int             isBreak;
    int             fullInit;
    int             nonConstIter;
    int             state;
    int             hasNoUnevenExecutionPath;
    int             builtInIter;
    int             isBroken;
};

typedef struct boolean_e_c bool;

// TODO: create functions for erasing variable (with free) and deep copy of memory (may be used in cycles).
struct memory_functions_t {
    // inits memory
    void (*init)(Memory *);

    // destroy memory
    void (*destroy)(Memory *);

    // returns pointer to register by variable varType
    Register *(*getRegister)(Memory *, VariableType);

    // adds new variable to register
    void (*newNum)(Memory *, char *, Variant);

    // adds new variable to register
    void (*newString)(Memory *, char *, Variant);

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

#define DESTROY_MEMORY(MEM)  MemoryFunctions.destroy(&MEM)

#define MEMORY_NEW_STR(MEMORY, NAME, VALUE) { \
Variant t = {.d = strlen(VALUE), .s = VALUE,.isOverflowed = 0, .varType = String}; \
MemoryFunctions.newString(&MEMORY, NAME, t); \
}

#define MEMORY_NEW_NUM(MEMORY, TYPE, NAME, VALUE) { \
Variant t = {.d = VALUE,.isOverflowed = 0, .numType = TYPE, .varType = Numerical}; \
MemoryFunctions.newNum(&MEMORY, NAME, t); \
}

#endif //ADVANCED_AUTOFORMATTER_MEMORY_H
