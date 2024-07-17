#pragma once

#include "imgio.h"
#include "mystdlib.h"

#include <stdbool.h>

struct path {
    ivec2* points;
    unsigned len;
    inline bool isValid() {
        return points != NULL;
    };
};

path aStar(const image navmesh, ivec2 start, ivec2 goal, bool isWall(const rgb));
