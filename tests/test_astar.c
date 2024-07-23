#include "test_main.h"
#define TEST_NO_MAIN
#include "acutest.h"

#include "imgio.h"
#include "nav/pathfind.h"

#include <string.h>

bool hasMaxRed(rgb x) { return x.r == 255; }

void testMaze(const char* fileName, const ivec2 start, const ivec2 goal, const unsigned nodeCount) {
    constexpr rgb red = (rgb) {255, 0, 0};
    
    image img = imgio_readPPM(fileName);
    path p = aStar(graph<int>{img, hasMaxRed}, start, goal);
    MY_TC(p.nodes.size() == nodeCount);
    MY_TC(p.length() == nodeCount-1);
    for(auto& node : p.nodes) {
        rgb& pathInImg = rgbAt(img, node.pos.x, node.pos.y);
        MY_TC(pathInImg == red);
        pathInImg.r = 0;
    }
    for(unsigned i = 0; i < img.width*img.height; i++) {
        MY_TC(img.pixels[i].r == img.pixels[i].g && img.pixels[i].g == img.pixels[i].b);
    }
    free(img.pixels);
}

void test_astar_maze1() { testMaze("resources/test/maze1.ppm", (ivec2){0, 1}, (ivec2){40,39}, 187); }
void test_astar_maze2() { testMaze("resources/test/maze2.ppm", (ivec2){0,19}, (ivec2){40,21}, 187); }
void test_astar_maze3() { testMaze("resources/test/maze3.ppm", (ivec2){19,0}, (ivec2){21,40}, 275); }
void test_astar_mazeTripleWeight() {
    constexpr rgb red = (rgb) {255, 0, 0};
    
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){0, 1}, (ivec2){40,39}, [](auto, auto){return 17;});
    MY_TC(p.nodes.size() == 187);
    MY_TC(p.length() == 186 * 17);
    for(auto& node : p.nodes) {
        rgb& pathInImg = rgbAt(img, node.pos.x, node.pos.y);
        MY_TC(pathInImg == red);
        pathInImg.r = 0;
    }
    for(unsigned i = 0; i < img.width*img.height; i++) {
        MY_TC(img.pixels[i].r == img.pixels[i].g && img.pixels[i].g == img.pixels[i].b);
    }
    free(img.pixels);

}
void test_astar_mazeNoPath() {
    image img = imgio_readPPM("resources/test/maze4.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){19,0}, (ivec2){21,40});
    MY_TC(!p.isValid());
    free(img.pixels);
}
void test_astar_mazeStartInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){0,0}, (ivec2){40,39});
    MY_TC(!p.isValid());
    free(img.pixels);
}
void test_astar_mazeGoalInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){0,1}, (ivec2){40,38});
    MY_TC(!p.isValid());
    free(img.pixels);
}
void test_astar_mazeStartAndGoalInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){0,2}, (ivec2){40,40});
    MY_TC(!p.isValid());
    free(img.pixels);
}
void test_astar_startInGoal() {
    image img = imgio_readPPM("resources/test/maze4.ppm");
    path p = aStar(graph<int>{img, hasMaxRed}, (ivec2){19,0}, (ivec2){19,0});
    MY_TC(p.nodes.size() == 1);
    MY_TC(p.nodes[0].weight == 0);
    MY_TC((p.nodes[0].pos == (ivec2){19,0}));
    free(img.pixels);
}
