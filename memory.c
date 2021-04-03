#include "memory.h"

void InitRegister(Register *r) {
    r->total = 0;
    r->items = (Variant *) malloc(ARRAY_LEN * sizeof(Variant));
    r->names = (char **) malloc(ARRAY_LEN * sizeof(char *));
    for (int i = 0; i < ARRAY_LEN; ++i) {
        r->names[i] = (char *) malloc(MAX_STRING_LEN * sizeof(char));
        memset(r->names[i], 0, MAX_STRING_LEN);
    }
}

#define INIT_REGISTER(REG) Register REG; \
InitRegister(&REG)

#pragma region FunctionPrototypes

int find(char **arr, int size, char *key) {
    for (int i = 0; i < size; ++i) {
        if (!strcmp(arr[i], key)) {
            return i;
        }
    }
    return size;
}

Register *getRegister(Memory *m, VarType type) {
    for (int i = 0; i < m->total; ++i) {
        if (m->type[i] == type) {
            return &m->registers[i];
        }
    }
    return NULL;
}

void new(Memory *m, VarType type, char *name, Variant item) {
    Register *r = getRegister(m, type);
    if (r == NULL) {
        m->type[m->total] = type;
        r = &m->registers[m->total];
        m->total++;
    }
    assert(r != NULL && "NULL in memory register.");

    // now r contains certain register

    int i = find(r->names, r->total, name);

    // new value will be added into [r->total], so r->total++
    if (i == r->total) {
        r->total++;
    }

    strcpy(r->names[i], name);
    r->items[i] = item;
}

void printRegister(Memory *m, VarType type) {
    Register *r = getRegister(m, type);

    switch (type) {
        case Int:
            printf("Register<Int>\n");
            break;
        case Double:
            printf("Register<Double>\n");
            break;
        case String:
            printf("Register<String>\n");
            break;
        default:
            printf("Bad Register<...>\n");
            return;
    }

    for (int i = 0; i < r->total; ++i) {
        switch (r->items[i].type) {
            case Int:
                printf("[%2d] %s = %d\n", i, r->names[i], r->items[i].i);
                break;
            case Double:
                printf("[%2d] %s = %f\n", i, r->names[i], r->items[i].d);
                break;
            case String:
                printf("[%2d] %s = %s\n", i, r->names[i], r->items[i].s);
                break;
        }
    }
}


Variant *getValue(Memory *m, char *name) {
    for (int regi = 0; regi < m->total; ++regi) {
        Register *r = &m->registers[regi];
        for (int vari = 0; vari < r->total; ++vari) {
            int i = find(r->names, r->total, name);
            if (i != r->total) {
                return &r->items[i];
            }
        }
    }
    return NULL;
}

void inc(Memory *m, char *Varname){
    Variant *tmp = getValue(m,Varname);
    switch(tmp->type){
        case Int:
            tmp->i +=1;
        case Double:
            tmp->d +=1.0;
        default:
            break;
    }
}

void dec(Memory *m, char *Varname){
    Variant *tmp = getValue(m,Varname);
    switch(tmp->type){
        case Int:
            tmp->i -=1;
        case Double:
            tmp->d -=1.0;
        default:
            break;
    }
}

#pragma endregion FunctionPrototypes

void InitMemory(Memory *m) {
    m->total = 0;
    for (int i = 0; i < REGISTERS_COUNT; ++i) {
        InitRegister(&m->registers[i]);
    }
}

struct memory_functions_t MemoryFunctions = {
        InitMemory,
        getRegister,
        new,
        printRegister,
        getValue,
        inc,
        dec
};