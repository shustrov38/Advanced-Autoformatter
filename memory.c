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

Register *getRegister(Memory *m, size_t size) {
    for (int i = 0; i < m->total; ++i) {
        if (m->size[i] == size) {
            return &m->registers[i];
        }
    }
    return NULL;
}

int find(char **arr, int *size, char *key) {
    int n = *size;
    for (int i = 0; i < n; ++i) {
        if (!strcmp(arr[i], key)) {
            return i;
        }
    }
    *size = *size + 1;
    return n;
}

void new(Memory *m, size_t size, char *name, Variant item) {
    Register *r = m->getRegister(m, size);
    if (r == NULL) {
        m->size[m->total] = size;
        r = &m->registers[m->total];
        m->total++;
    }
    assert(r != NULL && "NULL in memory register.");

    // now r contains certain register
    int i = find(r->names, &r->total, name);

    strcpy(r->names[i], name);
    r->items[i] = item;
}

void printRegister(Memory *m, size_t size) {
    Register *r = getRegister(m, size);

    switch (size) {
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

void InitMemory(Memory *m) {
    m->total = 0;
    for (int i = 0; i < REGISTERS_COUNT; ++i) {
        InitRegister(&m->registers[i]);
    }
    m->getRegister = getRegister;
    m->new = new;
    m->printRegister = printRegister;
}