#pragma once

#include <string.h>
#include <assert.h>

#define member(type, member) (((type *)0)->member)
#define arrlen(arr) (sizeof(arr)/sizeof(arr[0]))
#define structeq(obj1, obj2)({ \
    _Static_assert( \
        __builtin_types_compatible_p(__typeof__(obj1), __typeof__(obj2)), \
        "Mismatched types" \
    ); \
    memcmp(&obj1, &obj2, sizeof(obj1)) == 0; \
})

typedef struct {
    int x;
    int y;
} ivec2;

void perrorExit(const char* msg, int errorValue);
void memswap(void* A, void* B, size_t size);
int taxicabDist(const ivec2 A, const ivec2 B);
