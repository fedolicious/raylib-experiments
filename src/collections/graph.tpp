#pragma once

template<typename T>
bool graph<T>::node::operator<(const ivec2 other) const {
    const auto cmp{this->pos.y <=> other.y};
    if(cmp < 0) { return true; }
    if(cmp > 0) { return false; }
    return this->pos.x < other.x;
}
template <typename T>
graph<T>::node::node(const ivec2 p, const T&& d):
    data{d},
    neighborIndices{},
    pos{p}
{}
template<typename T> template<class Func>
graph<T>::graph(const image& img, Func is_in_graph):
    nodes{}
{
    using iter = std::vector<graph<T>::node>::iterator;
    constexpr std::array<ivec2, 4> neighborOffsets{
        ivec2{-1, 0},
        ivec2{1, 0},
        ivec2{0, -1},
        ivec2{0, 1},
    };
    auto& nodes = this->nodes;
    for(unsigned i = 0; i < img.width*img.height; i++) {
        const ivec2 pos{
            static_cast<int>(i%img.width),
            static_cast<int>(i/img.width)
        };
        if(!is_in_graph(img.pixels[i])) { continue; }
        
        nodes.emplace_back(graph::node{pos, T{}});
        const iter thisNode = lower_bound(nodes.begin(), nodes.end(), pos);
        assert(thisNode->pos == pos);   
        for(const auto& offset : neighborOffsets) {
            const iter neighbor = lower_bound(nodes.begin(), nodes.end(), pos+offset);
            if(neighbor == nodes.end() || neighbor->pos != pos+offset) { continue; }
            thisNode->neighborIndices.push_back(std::distance(nodes.begin(), neighbor));
            neighbor->neighborIndices.push_back(std::distance(nodes.begin(), thisNode));
        }
    }
}
