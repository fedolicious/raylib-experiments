#include "prioq.h"

#include "mystdlib.h"

#include <stdlib.h>
#include <string.h>

prioq* new_prioq() {
    prioq* obj = malloc(sizeof(prioq));
    *obj = (prioq) {
        .elements = malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*member(prioq, elements)) ),
        .priorities = malloc(DEFAULT_PRIOQ_CAPAC * sizeof(*member(prioq, priorities)) ),
        .capac = DEFAULT_PRIOQ_CAPAC,
        .len = 0,
    };
    return obj;
}
void delete_prioq(prioq* queue) {
    free(queue->elements);
    free(queue->priorities);
    free(queue);
}

int prioq_setCapac(prioq* queue, int capac) {
    queue->capac = capac;
    queue->elements = realloc(queue->elements, capac * sizeof(*member(prioq, elements)) );
    if(queue->elements == NULL) { return -1; }
    queue->priorities = realloc(queue->priorities, capac * sizeof(*member(prioq, priorities)) );
    if(queue->priorities == NULL) { return -1; }
    return 0;
}
int prioq_add(prioq* queue, prioqElt elt, int priority) {
    if(queue->len >= queue->capac) {
        int ret = prioq_setCapac(queue, queue->capac*2);
        if(ret != 0) { return ret; }
    }
    queue->elements[queue->len] = elt;
    queue->priorities[queue->len] = priority;
    queue->len++;
    return 0;
}
int prioq_removeIndex(prioq* queue, int index) {
    unsigned halfCapac = queue->capac/2;
    if(queue->len <= halfCapac) {
        int ret = prioq_setCapac(queue, halfCapac);
        if(ret != 0) { return ret; }
    }
    memmove(
        &queue->elements[index], &queue->elements[index+1],
    (queue->len - (index+1))*sizeof(*queue->elements));
    queue->len--;
    return 0;
    
}
int prioq_peekIndex(prioq* queue) {
    if(queue->len == 0) { return 0; }
    int headIndex = 0;
    for(int i = 1; i < queue->len; i++) {
        if(queue->priorities[i] > queue->priorities[headIndex]) {
            headIndex = i;
        }
    }
    return headIndex;
}
//undefined behaviour if queue is empty
prioqElt prioq_poll(prioq* queue) {
    int headIndex = prioq_peekIndex(queue);
    prioqElt ret = queue->elements[headIndex];
    prioq_removeIndex(queue, headIndex);
    return ret;
}
prioqElt prioq_peek(prioq* queue) {
    return queue->elements[prioq_peekIndex(queue)];   
}
