#include "test_main.h"
#define TEST_NO_MAIN
#include "acutest.h"

#include "collections/graph.h"
#include "imgio.h"

#include <unordered_set>
#include <algorithm>

static bool isWhite(rgb x) { return x == rgb{255, 255, 255}; }
static int zeroFn(rgb) { return 0; }

void test_graph_loops() {
    image img = imgio_readPPM("resources/test/loops.ppm");
    graph<int> g{img, isWhite, zeroFn};
    MY_TC(g.nodes.size() == 32);
    for(const auto& node : g.nodes) {
        MY_TC(node.neighborIndices.size() == 2);
    }
    
    std::unordered_set<decltype(g.nodes)::value_type*> nodeChain{};
    for(std::size_t nodeCount = 8; nodeCount <= 32; nodeCount += 8) {
        auto node = find_if(
            g.nodes.begin(), g.nodes.end(),
            [&](auto& x){
                return !nodeChain.contains(&x);
            }
        );
        nodeChain.insert(&*node);
        for(int i = 0; i < 7; i++) {
            const auto nextNodeIndex = find_if(
                node->neighborIndices.begin(),
                node->neighborIndices.end(),
                [&](auto x){
                    return !nodeChain.contains(&g.nodes.at(x));
                }
            );
            MY_TC(nextNodeIndex != node->neighborIndices.end());
            node = g.nodes.begin() + *nextNodeIndex;
            nodeChain.insert(&*node);
        }
        const auto end = find_if(
            node->neighborIndices.begin(),
            node->neighborIndices.end(),
            [&](auto x){
                return !nodeChain.contains(&g.nodes.at(x));
            }
        );
        MY_TC(end == node->neighborIndices.end());
        MY_TC(nodeChain.size() == nodeCount);
    }
    free(img.pixels);
}
void test_graph_blank() {
    image img = imgio_readPPM("resources/test/blank.ppm");
    graph<float> g{img, isWhite, zeroFn};
    MY_TC(g.nodes.size() == 0);
    free(img.pixels);
}
