#pragma once

#include "mystdlib.h"
#include "imgio.h"

#include <array>
#include <vector>

template <typename T>
class graph {
public:
    class node {
    public:
        T data;
        std::vector<std::size_t> neighborIndices;
        ivec2 pos;
        node() = delete;
        node(const ivec2, const T&&);
        bool operator<(const ivec2 other) const;
    };
    typedef T member_type;
    std::vector<graph<T>::node> nodes;
    template<class InGraphFunc, class ConstructionFunc>
    graph(const image&, InGraphFunc, ConstructionFunc);
};
#include "graph.tpp"
