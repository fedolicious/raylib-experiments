#pragma once

template<typename T>
bool graph<T>::node::operator<(const ivec2 other) const {
    const auto cmp{this->pos.y <=> other.y};
    if(cmp < 0) { return true; }
    if(cmp > 0) { return false; }
    return this->pos.x < other.x;
}
template <typename T>
graph<T>::node::node(const ivec2 pos, const T&& t):
    data{t},
    neighborIndices{},
    pos{pos}
{}
template<typename T> template<class InGraphFunc, class ConstructionFunc>
graph<T>::graph(const image& img, InGraphFunc is_in_graph, ConstructionFunc construct):
    nodes{}
{
    static_assert(std::is_same<bool, std::invoke_result_t<InGraphFunc,rgb>>::value);
    using iter = std::vector<graph<T>::node>::iterator;
    constexpr std::array<ivec2, 4> neighborOffsets{
        ivec2{-1, 0},
        ivec2{1, 0},
        ivec2{0, -1},
        ivec2{0, 1},
    };
    auto& nodes = this->nodes;
    for(unsigned i = 0; i < img.width*img.height; i++) {
        if(!is_in_graph(img.pixels[i])) { continue; }
        const ivec2 pos{
            static_cast<int>(i%img.width),
            static_cast<int>(i/img.width)
        };
        nodes.emplace_back(pos, std::move(construct(img.pixels[i])));
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
