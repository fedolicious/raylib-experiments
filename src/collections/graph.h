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
        std::vector<typename decltype(graph<T>::nodes)::size_type> neighborIndices;
        ivec2 pos;
        node() = delete;
        node(const ivec2, const T&&);
        bool operator<(const ivec2 other) const;
    };
    typedef T member_type;
    std::vector<graph<T>::node> nodes;
    template<typename Func>
    graph(const image&, Func);
};
#include "graph.tpp"
