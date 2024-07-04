#pragma once

#include "imgio.h"
#include "mystdlib.h"

typedef struct {
    ivec2* points;
    int len;
} path;

path aStar(const image navmesh, ivec2 start, ivec2 goal);