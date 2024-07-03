#pragma once

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
    prioq_##name* queue = malloc(sizeof(prioq_##name));                                  \
    *queue = (prioq_##name) {                                                            \
        .elements = malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*queue->elements)),              \
        .compare = cmpFn,                                                                \
        .capac = DEFAULT_PRIOQ_CAPAC,                                                    \
        .len = 0,                                                                        \
    };                                                                                   \
    return queue;                                                                        \
}                                                                                        \
void delete_prioq_##name(prioq_##name* queue) {                                          \
    for(int i = 0; i < queue->len; i++) {                                                \
        delete_##name(queue->elements[i]);                                               \
    }                                                                                    \
    free(queue->elements);                                                               \
    free(queue);                                                                         \
}                                                                                        \
int prioq_setCapac(prioq_##name* queue, int capac) {                                     \
    queue->capac = capac;                                                                \
    queue->elements = realloc(queue->elements, capac * sizeof(*queue->elements));        \
    if(queue->elements == NULL) { return -1; }                                           \
    return 0;                                                                            \
}                                                                                        \
int prioq_##name##_add(prioq_##name* queue, T elt) {                                     \
    if(queue->len >= queue->capac) {                                                     \
        int ret = prioq_setCapac(queue, queue->capac*2);                                 \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    queue->elements[queue->len] = elt;                                                   \
    queue->len++;                                                                        \
    return 0;                                                                            \
}                                                                                        \
                                                                                         \
int prioq_removeIndex(prioq_##name* queue, int index) {                                  \
    if(queue->len <= queue->capac/4) {                                                   \
        int ret = prioq_setCapac(queue, queue->capac/2);                                 \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    memmove(                                                                             \
        &queue->elements[index], &queue->elements[index+1],                              \
    (queue->len - (index+1))*sizeof(*queue->elements));                                  \
    queue->len--;                                                                        \
    return 0;                                                                            \
                                                                                         \
}                                                                                        \
int prioq_peekIndex(prioq_##name* queue) {                                               \
    if(queue->len == 0) { return 0; }                                                    \
    int headIndex = 0;                                                                   \
    for(int i = 1; i < queue->len; i++) {                                                \
        const int cmp = queue->compare(queue->elements[headIndex], queue->elements[i]);  \
        if(cmp > 0) { headIndex = i; }                                                   \
    }                                                                                    \
    return headIndex;                                                                    \
}                                                                                        \
T prioq_##name##_poll(prioq_##name* queue) {                                             \
    int headIndex = prioq_peekIndex(queue);                                              \
    T ret = queue->elements[headIndex];                                                  \
    prioq_removeIndex(queue, headIndex);                                                 \
    return ret;                                                                          \
}                                                                                        \
T prioq_##name##_peek(prioq_##name* queue) {                                             \
    return queue->elements[prioq_peekIndex(queue)];                                      \
}
