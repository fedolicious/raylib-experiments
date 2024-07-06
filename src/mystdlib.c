#include "mystdlib.h"

#include <stdlib.h>

void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}

inline int taxicabDist(const ivec2 A, const ivec2 B) {
    return abs(A.x-B.x) + abs(A.y-B.y);
}