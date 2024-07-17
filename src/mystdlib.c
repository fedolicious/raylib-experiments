#include "mystdlib.h"

#include <stdlib.h>
#include <assert.h>

bool ivec2::operator==(const ivec2& other) {
    return x == other.x && y == other.y;
}
bool ivec2::operator!=(const ivec2& other) {
    return !(*this == other);
}


void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}
