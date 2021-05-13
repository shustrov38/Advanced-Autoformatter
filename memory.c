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
    if (key != NULL && strlen(key) != 0) {
//        printf("[%s]\n", key);
        for (int i = 0; i < size; ++i) {
            if (!strcmp(arr[i], key)) {
                return i;
            }
        }
    }
    return size;
}

Register *getRegister(Memory *m, VariableType type) {
    for (int i = 0; i < m->total; ++i) {
        if (m->type[i] == type) {
            return &m->registers[i];
        }
    }
    return NULL;
}

void newNum(Memory *m, char *name, Variant item) {
    Register *r = getRegister(m, Numerical);
    if (r == NULL) {
        m->type[m->total] = Numerical;
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
    r->items[i].isInited = 0;
}

void newString(Memory *m, char *name, Variant item) {
    Register *r = getRegister(m, String);
    if (r == NULL) {
        m->type[m->total] = String;
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

void printRegister(Memory *m, VariableType type) {
    Register *r = getRegister(m, type);

    switch (type) {
        case Numerical:
            printf("Register<Numerics>\n");
            break;
        case String:
            printf("Register<String>\n");
            break;
        default:
            printf("Bad Register<...>\n");
            return;
    }

    for (int i = 0; i < r->total; ++i) {
        switch (r->items[i].varType) {
            case Numerical:
                switch (r->items[i].numType) {
                    case Int:
                        printf("[%2d] integer %s = %d\n", i, r->names[i], (int) r->items[i].d);
                        break;
                    case Double:
                        printf("[%2d] float %s = %f\n", i, r->names[i], r->items[i].d);
                        break;
                    case Unsigned:
                        if (r->items[i].d < 0) {
                            printf("[%2d] unsigned %s is overflowed\n", i, r->names[i]);
                            break;
                        } else {
                            printf("[%2d] unsigned %s = %d\n", i, r->names[i], (int) r->items[i].d);
                            break;
                        }
                }
                break;
            case String:
                printf("[%2d] string %s = %s\n", i, r->names[i], r->items[i].s);
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
            } else{
                break;
            }
        }
    }
    return getValue(m,"?nTmp");
}

void inc(Memory *m, char *Varname) {
    Variant *tmp = getValue(m, Varname);
    switch (tmp->numType) {
        case Int:
            tmp->d += 1;
            break;
        case Double:
            tmp->d += 1.0;
            break;
        case Unsigned:
            tmp->d += 1;
            break;
        default:
            break;
    }
}

void dec(Memory *m, char *Varname) {
    Variant *tmp = getValue(m, Varname);
    switch (tmp->numType) {
        case Int:
            tmp->d -= 1;
            break;
        case Double:
            tmp->d -= 1.0;
            break;
        case Unsigned:
            tmp->d -= 1;
            break;
        default:
            break;
    }
}

void overflowChecker(Memory *m) {
    Register *tmp = MemoryFunctions.getRegister(m, Numerical);
    if (tmp == NULL) {
        return;
    }
    for (int i = 0; i < tmp->total; ++i) {
        if (tmp->items[i].numType == Unsigned && tmp->items[i].d < 0) {
            tmp->items[i].isOverflowed = 1;
        }
    }
}

#pragma endregion FunctionPrototypes

void InitMemory(Memory *m) {
    m->total = 0;
    for (int i = 0; i < REGISTERS_COUNT; ++i) {
        InitRegister(&m->registers[i]);
    }
}

void DestroyMemory(Memory *m) {
    for (int i = 0; i < REGISTERS_COUNT; ++i) {
        for (int j = 0; j < ARRAY_LEN; ++j) {
            free(m->registers[i].names[j]);
        }
        free(m->registers[i].items);
        free(m->registers[i].names);
    }
    free(&m);
}

struct memory_functions_t MemoryFunctions = {
        InitMemory,
        DestroyMemory,
        getRegister,
        newNum,
        newString,
        printRegister,
        getValue,
        inc,
        dec,
        overflowChecker
};