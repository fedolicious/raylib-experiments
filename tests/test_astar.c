#include "test_main.h"
#define TEST_NO_MAIN
#include "acutest.h"

#include "imgio.h"
#include "nav/pathfind.h"

#include <string.h>
void testMaze(const char* fileName, const ivec2 start, const ivec2 goal, const int pathLen) {
    const rgb red = (rgb) {255, 0, 0};
    
    image img = imgio_readPPM(fileName);
    path p = aStar(img, start, goal);
    MY_TC(p.len == pathLen);
    for(int i = 0; i < p.len; i++) {
        rgb* pathInImg = &rgbAt(img, p.points[i].x, p.points[i].y);
        MY_TC(memcmp(pathInImg, &red, sizeof(rgb)) == 0);
        pathInImg->r = 0;
    }
    for(int i = 0; i < img.width*img.height; i++) {
        MY_TC(img.pixels[i].r == img.pixels[i].g && img.pixels[i].g == img.pixels[i].b);
    }
    
    free(p.points);
    free(img.pixels);
    
}

void test_astar_maze1() { testMaze("resources/test/maze1.ppm", (ivec2){0, 1}, (ivec2){40,39}, 187); }
void test_astar_maze2() { testMaze("resources/test/maze2.ppm", (ivec2){0,19}, (ivec2){40,21}, 187); }
void test_astar_maze3() { testMaze("resources/test/maze3.ppm", (ivec2){19,0}, (ivec2){21,40}, 275); }
void test_astar_mazeNoPath() {
    image img = imgio_readPPM("resources/test/maze4.ppm");
    path p = aStar(img, (ivec2){19,0}, (ivec2){21,40});
    MY_TC(!path_isValid(p));
    free(img.pixels);
}
void test_astar_mazeStartInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(img, (ivec2){0,0}, (ivec2){40,39});
    MY_TC(!path_isValid(p));
    free(img.pixels);
}
void test_astar_mazeGoalInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(img, (ivec2){0,1}, (ivec2){40,38});
    MY_TC(!path_isValid(p));
    free(img.pixels);
}
void test_astar_mazeStartAndGoalInWall() {
    image img = imgio_readPPM("resources/test/maze1.ppm");
    path p = aStar(img, (ivec2){0,2}, (ivec2){40,40});
    MY_TC(!path_isValid(p));
    free(img.pixels);
}
