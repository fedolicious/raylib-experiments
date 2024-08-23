#include "world_map.h"

#include <unordered_set>
#include <queue>

bool notBlack(rgb x) { return x != rgb{0, 0, 0}; }
inline world_map::node construct_wm_node(rgb x) { return world_map::node(x); }

world_map::node::node(rgb x):
    in_subgraph(x == rgb(0,0,255)),
    wall(false),
    enemy0_mask(false),
    enemy1_mask(false) {}
world_map::world_map(image img):
    graph(img, notBlack, construct_wm_node) {

    for(std::size_t i = 0; i < graph.nodes.size(); i++) {
        std::unordered_set<std::size_t> visited{};
        std::queue<std::size_t> unvisited{};
        unvisited.push(i);
        while(!graph.nodes.at(unvisited.front()).data.in_subgraph) {
            assert(!visited.contains(unvisited.front()));
            for(const auto& neighborIndex : graph.nodes.at(unvisited.front()).neighborIndices) {
                if(visited.contains(neighborIndex)) { continue; }
                unvisited.push(neighborIndex);
            }
            unvisited.pop();
            assert(!unvisited.empty());
        }
        graph.nodes.at(i).data.closest_subgraph_node_indx = unvisited.front();
    }
}
world_map::graph_nodes_iter world_map::nodeAt(ivec2 x) {
    return find_if(graph.nodes.begin(), graph.nodes.end(), [&](const auto& y){return y.pos == x;}); //TODO binary search (optimization)
}
