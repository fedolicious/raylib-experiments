#pragma once

#include "imgio.h"
#include "collections/graph.h"

class world_map {
public:
    struct node {
        std::size_t closest_subgraph_node_indx;
        bool in_subgraph;
        bool wall;
        bool enemy0_mask;
        bool enemy1_mask;
        explicit node(rgb);
    };
    typedef std::vector<graph<world_map::node>::node>::iterator graph_nodes_iter;
    explicit world_map(image);
    graph_nodes_iter nodeAt(ivec2);

    graph<node> graph;
};
