#include "vector.h"

int vectorTotal(vector *v) {
    int totalCount = UNDEFINE;
    if (v) {
        totalCount = v->total;
    }
    return totalCount;
}

int vectorResize(vector *v, int capacity) {
    int status = UNDEFINE;
    if (v) {
        void **items = realloc(v->items, sizeof(void *) * capacity);
        if (items) {
            v->items = items;
            v->capacity = capacity;
            status = SUCCESS;
        }
    }
    return status;
}

int vectorPushBack(vector *v, void *item) {
    int status = UNDEFINE;
    if (v) {
        if (v->capacity == v->total) {
            status = vectorResize(v, v->capacity * 2);
            if (status != UNDEFINE) {
                v->items[v->total++] = item;
            }
        } else {
            v->items[v->total++] = item;
            status = SUCCESS;
        }
    }
    return status;
}

int vectorSet(vector *v, int index, void *item) {
    int status = UNDEFINE;
    if (v) {
        if ((index >= 0) && (index < v->total)) {
            v->items[index] = item;
            status = SUCCESS;
        }
    }
    return status;
}

void *vectorGet(vector *v, int index) {
    void *readData = NULL;
    if (v) {
        if ((index >= 0) && (index < v->total)) {
            readData = v->items[index];
        }
    }
    return readData;
}

int vectorDelete(vector *v, int index) {
    int status = UNDEFINE;
    int i = 0;
    if (v) {
        if ((index < 0) || (index >= v->total))
            return status;
        v->items[index] = NULL;
        for (i = index; (i < v->total - 1); ++i) {
            v->items[i] = v->items[i + 1];
            v->items[i + 1] = NULL;
        }
        v->total--;
        if ((v->total > 0) && ((v->total) == (v->capacity / 4))) {
            vectorResize(v, v->capacity / 2);
        }
        status = SUCCESS;
    }
    return status;
}

int vectorFree(vector *v) {
    int status = UNDEFINE;
    if (v) {
        free(v->items);
        v->items = NULL;
        status = SUCCESS;
    }
    return status;
}

void vector_init(vector *v) {
    //initialize the capacity and allocate the memory
    v->capacity = VECTOR_INIT_CAPACITY;
    v->total = 0;
    v->items = malloc(sizeof(void *) * v->capacity);
}

struct sVector Vec = {
        vectorTotal,
        vectorResize,
        vectorPushBack,
        vectorSet,
        vectorGet,
        vectorDelete,
        vectorFree
};