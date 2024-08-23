#pragma once

template <typename AStarData>
path::path(const AStarData& data, const AStarData::graph_type::node& lastNode): nodes{} {
    const auto* node = &lastNode;
    while(data.data_of(*node).came_from != nullptr) {
        const auto* prevNode = data.data_of(*node).came_from;
        assert(data.data_of(*node).g_val > data.data_of(*prevNode).g_val);
        this->nodes.emplace_back(
            node->pos,
            data.data_of(*node).g_val - data.data_of(*prevNode).g_val
        );
        node = prevNode;
    }
    this->nodes.emplace_back(node->pos, data.data_of(*node).g_val);
}

template <typename GraphT>
a_star_data<GraphT>::node_data::node_data():
    g_val{std::numeric_limits<decltype(g_val)>::max()},
    in_prioq{false}
{}
template <typename GraphT>
inline const a_star_data<GraphT>::graph_data a_star_data<GraphT>::graph_data_of(const GraphT::node& node) const {
    return a_star_data<GraphT>::graph_data{node.data, node.pos, this->data_of(node).g_val};
}
template <typename GraphT>
inline const a_star_data<GraphT>::node_data& a_star_data<GraphT>::data_of(const GraphT::node& node) const {
    return data[&node - graph.nodes.data()];
}
template <typename GraphT>
inline a_star_data<GraphT>::node_data& a_star_data<GraphT>::data_of(const GraphT::node& node) {
    return data[&node - graph.nodes.data()];
}
template <typename GraphT>
a_star_data<GraphT>::a_star_data(const GraphT& graph):
    graph{graph},
    data{new a_star_data<GraphT>::node_data[graph.nodes.size()]}
{}

template<typename T, class WeightFunc, class HeuristicFunc>
path a_star(
    const graph<T>& navGraph, const ivec2 start, const ivec2 goal,
    WeightFunc edgeWeight, HeuristicFunc heur
) { //TODO heuristic
    using node = typename graph<T>::node;
    
    if(start == goal) { return {goal}; }
    auto startIter = find_if(navGraph.nodes.begin(), navGraph.nodes.end(), [&](auto& x){return x.pos == start;});
    auto goalIter = find_if(navGraph.nodes.begin(), navGraph.nodes.end(), [&](auto& x){return x.pos == goal;});
    if(startIter == navGraph.nodes.end() || goalIter == navGraph.nodes.end()) {
        return {};
    }
    
    a_star_data aStarData{navGraph};
    
    const node& startNode = *startIter;
    auto& startData = aStarData.data_of(startNode);
    startData.came_from = nullptr;
    startData.g_val = 0;
    const node& goalNode = *goalIter;
    
    std::vector<const node*> nodes{&startNode};
    
    bool goalFound = false;
    const node* bestNode;
    while(!nodes.empty()) {
        auto minIter = std::min_element(nodes.begin(), nodes.end(),
            [&](const auto* A, const auto* B){
                const int AfVal = aStarData.data_of(*A).g_val + taxicabDist(A->pos, goal);
                const int BfVal = aStarData.data_of(*B).g_val + taxicabDist(B->pos, goal);
                return AfVal < BfVal;
            });
        assert(minIter != nodes.end());
        bestNode = *minIter;
        aStarData.data_of(*bestNode).in_prioq = false;
        nodes.erase(minIter);
        
        if(bestNode == &goalNode) {
            goalFound = true;
            break;
        }
        
        for(auto i : bestNode->neighborIndices) {
            const node& neighbor = navGraph.nodes.at(i);
            
            const unsigned contenderGVal =
                aStarData.data_of(*bestNode).g_val +
                edgeWeight(
                    aStarData.graph_data_of(*bestNode),
                    aStarData.graph_data_of(neighbor)
                );
            if(aStarData.data[i].g_val <= contenderGVal || contenderGVal < aStarData.data_of(*bestNode).g_val) {
                continue;
            }
            aStarData.data[i].g_val = contenderGVal;
            aStarData.data[i].came_from = bestNode;
            if(!aStarData.data[i].in_prioq) {
                nodes.push_back(&neighbor);
                aStarData.data[i].in_prioq = true;
            }
        }
    }
    if(!goalFound) {
        return {};
    } else {
        return path(aStarData, *bestNode);
    }
}
