#include "mystdlib.h"

#include <stdlib.h>
#include <assert.h>

void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}

inline void memswap(void* A, void* B, size_t size) {
    assert(abs((int)(A-B)) >= size);
    char C[size];
    memcpy(C, A, size);
    memcpy(A, B, size);
    memcpy(B, C, size);
}

inline int taxicabDist(const ivec2 A, const ivec2 B) {
    return abs(A.x-B.x) + abs(A.y-B.y);
}
