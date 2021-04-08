#ifndef ADVANCED_AUTOFORMATTER_VECTOR_H
#define ADVANCED_AUTOFORMATTER_VECTOR_H

#include <stdio.h>
#include <stdlib.h>

#define VECTOR_INIT_CAPACITY 6

#define UNDEFINE  -1
#define SUCCESS 0

#define INIT_VECTOR(vec) vector vec;\
 vector_init(&vec)

typedef struct sVectorList vector;
//Store and track the stored data
typedef struct sVectorList {
    void **items;
    int capacity;
    int total;
} sVectorList;
//structure contain the function pointer

struct sVector {
    int (*size)(vector *);

    int (*resize)(vector *, int);

    int (*push)(vector *, void *);

    int (*set)(vector *, int, void *);

    void *(*get)(vector *, int);

    int (*delete)(vector *, int);

    int (*free)(vector *);
};

extern struct sVector Vec;

void vector_init(vector *v);

#endif //ADVANCED_AUTOFORMATTER_VECTOR_H
