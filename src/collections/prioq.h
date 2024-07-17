#pragma once

#include <string.h>
#include <assert.h>

#define DEFAULT_PRIOQ_CAPAC 8

//You must provide a void delete_##name(prioq_##name) function when using DEFINE_PRIOQ(T, name);
#define DEFINE_PRIOQ(T, name)                                                            \
typedef struct {                                                                         \
    T* elements;                                                                         \
    int (*compare)(T, T);                                                                \
    unsigned capac;                                                                      \
    unsigned len;                                                                        \
} prioq_##name;                                                                          \
                                                                                         \
prioq_##name* new_prioq_##name(int(*cmpFn)(T, T)) {                                      \
    prioq_##name* queue = (prioq_##name*) malloc(sizeof(prioq_##name));                  \
    *queue = (prioq_##name) {                                                            \
        .elements = (T*) malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*queue->elements)),         \
        .compare = cmpFn,                                                                \
        .capac = DEFAULT_PRIOQ_CAPAC,                                                    \
        .len = 0,                                                                        \
    };                                                                                   \
    return queue;                                                                        \
}                                                                                        \
void delete_prioq_##name(prioq_##name* queue) {                                          \
    for(unsigned i = 0; i < queue->len; i++) {                                           \
        delete_##name(queue->elements[i]);                                               \
    }                                                                                    \
    free(queue->elements);                                                               \
    free(queue);                                                                         \
}                                                                                        \
inline int prioq_##name##_setCapac(prioq_##name* queue, unsigned capac) {                \
    queue->capac = capac;                                                                \
    queue->elements = (T*) realloc(queue->elements, capac * sizeof(*queue->elements));   \
    if(queue->elements == NULL) { return -1; }                                           \
    return 0;                                                                            \
}                                                                                        \
int prioq_##name##_add(prioq_##name* queue, T elt) {                                     \
    if(queue->len >= queue->capac) {                                                     \
        int ret = prioq_##name##_setCapac(queue, queue->capac*2);                        \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    queue->elements[queue->len] = elt;                                                   \
    queue->len++;                                                                        \
    return 0;                                                                            \
}                                                                                        \
                                                                                         \
int prioq_##name##_removeIndex(prioq_##name* queue, unsigned index) {                    \
    if(queue->len <= queue->capac/4) {                                                   \
        int ret = prioq_##name##_setCapac(queue, queue->capac/2);                        \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    memmove(                                                                             \
        &queue->elements[index], &queue->elements[index+1],                              \
    (queue->len - (int)(index+1))*sizeof(*queue->elements));                             \
    queue->len--;                                                                        \
    return 0;                                                                            \
                                                                                         \
}                                                                                        \
static inline unsigned prioq_##name##_peekIndex(prioq_##name* queue) {                   \
    unsigned headIndex = 0;                                                              \
    for(unsigned i = 1; i < queue->len; i++) {                                           \
        const int cmp = queue->compare(queue->elements[headIndex], queue->elements[i]);  \
        if(cmp > 0) { headIndex = i; }                                                   \
    }                                                                                    \
    return headIndex;                                                                    \
}                                                                                        \
T prioq_##name##_poll(prioq_##name* queue) {                                             \
    assert(queue->len > 0);                                                              \
    unsigned headIndex = prioq_##name##_peekIndex(queue);                                \
    T ret = queue->elements[headIndex];                                                  \
    prioq_##name##_removeIndex(queue, headIndex);                                        \
    return ret;                                                                          \
}                                                                                        \
T prioq_##name##_peek(prioq_##name* queue) {                                             \
    assert(queue->len > 0);                                                              \
    return queue->elements[prioq_##name##_peekIndex(queue)];                             \
}
