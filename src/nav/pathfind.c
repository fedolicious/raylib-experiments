#include "pathfind.h"

#include "collections/graph.h"

#include <numeric>

path::path_node::path_node(ivec2 p, unsigned w):
    pos{p},
    weight{w}
{}
path::path():
    nodes{}
{}
path::path(ivec2 pos):
    nodes{{pos, 0}}
{}
unsigned path::length() const {
    return std::accumulate(
        this->nodes.begin(), this->nodes.end(),
        0, [](const unsigned a, const auto& b){
            return a + b.weight;
        }
    );
}
