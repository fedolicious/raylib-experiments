#pragma once

#include "collections/graph.h"

#include <vector>
#include <algorithm>
#include <limits>
#include <cassert>
#include <memory>

class path {
    class path_node {
    public: 
        ivec2 pos;
        unsigned weight;
        path_node(ivec2, unsigned);
    };
public:
    std::vector<path_node> nodes; //nodes are stored from last to first
    inline bool isValid() const {
        return !nodes.empty();
    };
    path();
    path(ivec2);
    template <typename AStarData>
    path(const AStarData& data, const AStarData::graph_type::node& lastNode);
    unsigned length() const;
};

template <typename GraphT>
class a_star_data {
    struct node_data {
        const GraphT::node* came_from;
        unsigned g_val;
        bool in_prioq;
        node_data();
    };
    const GraphT& graph;
    inline auto data_size() { return graph.nodes.size(); }
        
public:
    struct graph_data {
        const GraphT::member_type& data;
        const ivec2 pos;
        const decltype(node_data::g_val) g_val;
    };
    inline const graph_data graph_data_of(const GraphT::node&) const;
    typedef GraphT graph_type;
    std::unique_ptr<node_data[]> data;
    inline const node_data& data_of(const GraphT::node&) const;
    inline node_data& data_of(const GraphT::node&);
    a_star_data(const GraphT& graph);
};
template <typename T, class WeightFunc =
    unsigned(*)(const typename a_star_data<graph<T>>::graph_data&, const typename a_star_data<graph<T>>::graph_data&),
class HeuristicFunc = int>
path a_star(
    const graph<T>&, const ivec2, const ivec2,
    WeightFunc = [](auto, auto)->unsigned{ return 1u; }, HeuristicFunc = 0
);
#include "pathfind.tpp"
