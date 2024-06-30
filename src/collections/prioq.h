#pragma once

typedef struct {
    int x;
    int y;
} prioqElt;
typedef struct {
    prioqElt* elements;
    int* priorities;
    unsigned capac;
    unsigned len;
} prioq;

#define DEFAULT_PRIOQ_CAPAC 8

prioq* new_prioq(void);
void delete_prioq(prioq* queue);
int prioq_add(prioq* queue, prioqElt elt, int priority);
prioqElt prioq_poll(prioq* queue);
prioqElt prioq_peek(prioq* queue);
