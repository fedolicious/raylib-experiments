#include "mystdlib.h"

bool ivec2::operator==(const ivec2 other) const {
    return x == other.x && y == other.y;
}
bool ivec2::operator!=(const ivec2 other) const {
    return !(*this == other);
}
ivec2 ivec2::operator+(const ivec2 other) const {
    return {this->x + other.x, this->y + other.y};
}
std::size_t std::hash<ivec2>::operator()(const ivec2& x) const {
    typedef long long hashable_t;
    static_assert(sizeof(hashable_t) == sizeof(ivec2));

    return std::hash<hashable_t>()(
        *reinterpret_cast<const hashable_t*>(&x)
    );
}

void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}
