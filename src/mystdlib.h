#pragma once

#include <cstring>
#include <cassert>
#include <functional>

#define member(type, member) (((type *)0)->member)

struct ivec2 {
    int x;
    int y;
    bool operator==(const ivec2 other) const;
    bool operator!=(const ivec2 other) const;
    ivec2 operator+(const ivec2 other) const;
};
template<> struct std::hash<ivec2> {
    std::size_t operator()(const ivec2& x) const;
};

void perrorExit(const char* msg, int errorValue);
inline void memswap(void* A, void* B, size_t size) {
    assert(
        static_cast<size_t>(abs(
            static_cast<char*>(A)
            -static_cast<char*>(B)
        )) >= size
    );
    char C[size];
    memcpy(C, A, size);
    memcpy(A, B, size);
    memcpy(B, C, size);
}
inline int taxicabDist(const ivec2 A, const ivec2 B) {
    return abs(A.x-B.x) + abs(A.y-B.y);
}
