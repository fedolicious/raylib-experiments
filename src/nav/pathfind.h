#pragma once

#include "imgio.h"
#include "mystdlib.h"

#include <stdbool.h>

typedef struct {
    ivec2* points;
    unsigned len;
} path;

path aStar(const image navmesh, ivec2 start, ivec2 goal, bool isWall(const rgb));
bool path_isValid(const path p);
