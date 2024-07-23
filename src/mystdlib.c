#include "mystdlib.h"

#include <stdlib.h>
#include <assert.h>

bool ivec2::operator==(const ivec2 other) const {
    return x == other.x && y == other.y;
}
bool ivec2::operator!=(const ivec2 other) const {
    return !(*this == other);
}
ivec2 ivec2::operator+(const ivec2 other) const {
    return {this->x + other.x, this->y + other.y};
}

void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}
