#pragma once

/*
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
*/
#define DEFAULT_PRIOQ_CAPAC 8                                                            

//MUST PROVIDE A void delete_##name(prioq_##name) FUNCTION
#define DEFINE_PRIOQ(T, name)                                                            \
typedef struct {                                                                         \
    T* elements;                                                                         \
    int* priorities;                                                                     \
    unsigned capac;                                                                      \
    unsigned len;                                                                        \
} prioq_##name;                                                                          \
                                                                                         \
prioq_##name* new_prioq_##name(void) {                                                   \
    prioq_##name* queue = malloc(sizeof(prioq_##name));                                  \
    *queue = (prioq_##name) {                                                            \
        .elements = malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*queue->elements)),              \
        .priorities = malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*queue->priorities)),          \
        .capac = DEFAULT_PRIOQ_CAPAC,                                                    \
        .len = 0,                                                                        \
    };                                                                                   \
    return queue;                                                                        \
}                                                                                        \
void delete_prioq_##name(prioq_##name* queue) {                                          \
    for(int i = 0; i < queue->len; i++) {                                                       \
        delete_##name(queue->elements[i]);                                               \
    }                                                                                    \
    free(queue->elements);                                                               \
    free(queue->priorities);                                                             \
    free(queue);                                                                         \
}                                                                                        \
int prioq_setCapac(prioq_##name* queue, int capac) {                                     \
    queue->capac = capac;                                                                \
    queue->elements = realloc(queue->elements, capac * sizeof(*queue->elements));        \
    if(queue->elements == NULL) { return -1; }                                           \
    queue->priorities = realloc(queue->priorities, capac * sizeof(*queue->priorities));  \
    if(queue->priorities == NULL) { return -1; }                                         \
    return 0;                                                                            \
}                                                                                        \
int prioq_##name##_add(prioq_##name* queue, T elt, int priority) {                       \
    if(queue->len >= queue->capac) {                                                     \
        int ret = prioq_setCapac(queue, queue->capac*2);                                 \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    queue->elements[queue->len] = elt;                                                   \
    queue->priorities[queue->len] = priority;                                            \
    queue->len++;                                                                        \
    return 0;                                                                            \
}                                                                                        \
                                                                                         \
int prioq_removeIndex(prioq_##name* queue, int index) {                                  \
    unsigned halfCapac = queue->capac/2;                                                 \
    if(queue->len <= halfCapac) {                                                        \
        int ret = prioq_setCapac(queue, halfCapac);                                      \
        if(ret != 0) { return ret; }                                                     \
    }                                                                                    \
    memmove(                                                                             \
        &queue->elements[index], &queue->elements[index+1],                              \
    (queue->len - (index+1))*sizeof(*queue->elements));                                  \
    memmove(                                                                             \
        &queue->priorities[index], &queue->priorities[index+1],                          \
    (queue->len - (index+1))*sizeof(*queue->priorities));                                \
    queue->len--;                                                                        \
    return 0;                                                                            \
                                                                                         \
}                                                                                        \
int prioq_peekIndex(prioq_##name* queue) {                                               \
    if(queue->len == 0) { return 0; }                                                    \
    int headIndex = 0;                                                                   \
    for(int i = 1; i < queue->len; i++) {                                                \
        if(queue->priorities[i] > queue->priorities[headIndex]) {                        \
            headIndex = i;                                                               \
        }                                                                                \
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
