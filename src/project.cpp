/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "imgio.h"
#include "collections/graph.h"
#include "nav/pathfind.h"

#include <iostream>
#include <filesystem>
#include <format>
#include <numeric>
#include <array>
#include <cassert>
#include <chrono>
#include <thread>

typedef struct {
    ivec2 pos;
} player;
struct enemy {
    ivec2 pos;
    path plannedPath;
    int stunTimer;
    enemy(ivec2 p) :
        pos{p},
        plannedPath{},
        stunTimer{0}
    {}
};
typedef struct {
    ivec2 pos;
} collectible;
ivec2 randomLocation(const image graph, const rgb locationType, const int locations) {

    int index = GetRandomValue(0, locations-1);
    for(unsigned i = 0; i < graph.width*graph.height; i++) {
        if(graph.pixels[i] != locationType) { continue; }
        if(index == 0) {
            return (ivec2) {
                .x = static_cast<int>(i%graph.width),
                .y = static_cast<int>(i/graph.width),
            };
        } else {
            index--;
        }
    }
    perrorExit("Mismatch between actual and provided number of valid locations", -1);
    return (ivec2){};
}
bool isNotWhite(const rgb x) {
    return !(x.r == 255 && x.g == 255 && x.b == 255);
}

int main(void) {
    const rgb white = (rgb){255,255,255};
    const rgb black = (rgb){0,0,0};
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    
    std::filesystem::current_path("resources");
    
    image map = imgio_readPPM("map.ppm");
    int whiteCells = 0;
    for(unsigned i = 0; i < map.width*map.height; i++) {
        if(memcmp(&map.pixels[i], &white, sizeof(rgb)) == 0) {
            whiteCells++;
        }
    }
    assert(whiteCells > 0);
    player player1{ivec2{46, 22}};
    std::array<enemy, 2> enemies{
        ivec2{10, 27},
        ivec2{20, 1},
    };
    
    if(memcmp(&rgbAt(map, player1.pos.x, player1.pos.y), &white, sizeof(rgb)) != 0) {
        perrorExit("Player starting location location must be open", -1);
    }
    
    int imgX = 10;
    int imgY = 10;
    int pxSize = 35;
    
    std::array<int,10> keysPressed;
    std::size_t numKeysPressed;
    
    int playerMoves = 0;
    int score = 0;
    int energy = 0;
    
    const int wallCost = 5;
    const int moveThreshold = 1;
    const int coinEnergy = 10;
    collectible coin;
    coin.pos = randomLocation(map, white, whiteCells);
    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        auto beginTime = std::chrono::high_resolution_clock::now();
        //zoom in/out
        if(IsKeyPressed(KEY_EQUAL)) { pxSize++; }
        if(IsKeyPressed(KEY_MINUS)) { pxSize--; }
        
        //move player
        ivec2 contenderPos = player1.pos;
        contenderPos.y +=
            IsKeyPressed(KEY_DOWN) + IsKeyPressed(KEY_S)
            - IsKeyPressed(KEY_UP) - IsKeyPressed(KEY_W);
        contenderPos.x +=
            IsKeyPressed(KEY_RIGHT) + IsKeyPressed(KEY_D)
            - IsKeyPressed(KEY_LEFT) - IsKeyPressed(KEY_A);
        if(taxicabDist(contenderPos, player1.pos) == 1) {
            if(rgbAt(map, contenderPos.x, contenderPos.y) == white) {
                player1.pos = contenderPos;
                playerMoves += 1;
            } else {
                //failed to move
            }
        }
        //place wall
        if(energy >= wallCost && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int rMouseX = (GetMouseX() - imgX);
            int rMouseY = (GetMouseY() - imgY);
            if(rMouseX >= 0 && rMouseY >= 0) {
                unsigned wallX = rMouseX/pxSize;
                unsigned wallY = rMouseY/pxSize;
                if(wallX < map.width && wallY < map.height && rgbAt(map, wallX, wallY).r == 255) {
                    energy -= wallCost;
                    rgbAt(map, wallX, wallY).r = 0;
                }
            }
        }
        class map_node {
        public:
            unsigned wall_health;
        };
        const graph<map_node> map_graph{
            map,
            [&](rgb x){ return x != black; },
            [](rgb x){ return map_node{unsigned((int(255-x.r)+59)/60)}; }
        };
        std::array<decltype(enemies)::value_type*, enemies.size()> enemyRefs;
        std::iota(enemyRefs.begin(), enemyRefs.end(), &*enemies.begin());
        
        //calculate each path to the player
        for(auto& enemy : enemies) { //TODO heuristic based on distance to other enemies
            enemy.plannedPath = a_star(map_graph, enemy.pos, player1.pos,
                [](const auto& a, const auto& b)->unsigned{
                    if(b.data.wall_health > a.g_val) {
                        return 1 + b.data.wall_health - a.g_val;
                    } else {
                        return 1;
                    }
                }
            );
        }
        /*
        //sort from best to worst path
        std::sort(enemyRefs.begin(), enemyRefs.end(),
            [](const auto* a, const auto* b){
                return a->plannedPath.length() < b->plannedPath.length();
            }
        );
        
        //recalculate each path, using the nodes that the shorter paths travel through as walls
        for(int i = 0; i < arrlen(enemyPaths)-1; i++) {
            //block off all parts of the path (except player location)
            if(!enemyPaths[i].isValid()) { continue; }
            for(int j = 0; j < enemyPaths[i].len-1; j++) {
                assert(memcmp(&rgbAt(map,
                    enemyPaths[i].points[j].x,
                    enemyPaths[i].points[j].y
                ), &white, sizeof(rgb)) == 0);
                rgbAt(map, enemyPaths[i].points[j].x, enemyPaths[i].points[j].y) = black;
            }
            //check if the next path intersects
            bool intersectsNextPath = false;
            if(!enemyPaths[i+1].isValid()) {continue; }
            for(int j = 0; j < enemyPaths[i+1].len; j++) {
                if(memcmp(&rgbAt(map,
                    enemyPaths[i+1].points[j].x,
                    enemyPaths[i+1].points[j].y
                ), &black, sizeof(rgb)) == 0) {
                    intersectsNextPath = true;
                    break;
                }
            }
            //if it intersects, recalculate it, otherwise do nothing
            if(intersectsNextPath) {
                free(enemyPaths[i+1].points);
                enemyPaths[i+1] = a_star(map, enemies[enemyIndxs[i+1]].pos, player1.pos, isNotWhite);
            }
        }
        //remove blockage
        for(int i = 0; i < arrlen(enemyPaths)-1; i++) {
            if(!enemyPaths[i].isValid()) { continue; }
            for(int j = 0; j < enemyPaths[i].len-1; j++) {
                assert(memcmp(&rgbAt(map,
                    enemyPaths[i].points[j].x,
                    enemyPaths[i].points[j].y
                ), &black, sizeof(rgb)) == 0);
                rgbAt(map, enemyPaths[i].points[j].x, enemyPaths[i].points[j].y) = white;
            }
        }
        */
        //move one step in the game
        if(IsKeyPressed(KEY_SPACE) || playerMoves >= moveThreshold) {
            //decay all walls
            for(unsigned i = 0; i < map.width*map.height; i++) {
                rgb* pixel = &map.pixels[i];
                if(pixel->g == 255 && pixel->r < 255) {
                    unsigned char newR = pixel->r + 60;
                    pixel->r = (newR < pixel->r)? 255 : newR;
                }
            }
            //move enemies along paths
            for(auto& enemy : enemies) {
                if(!enemy.plannedPath.isValid() || enemy.plannedPath.nodes.size() < 2) { continue; }
                if(enemy.stunTimer > 0) {
                    enemy.stunTimer --;
                    continue;
                }
                const auto& firstNode = *std::prev(enemy.plannedPath.nodes.end(), 1);
                const auto& secondNode = *std::prev(enemy.plannedPath.nodes.end(), 2);
                assert(firstNode.pos == enemy.pos);
                assert(taxicabDist(secondNode.pos, enemy.pos) == 1);
                if(secondNode.weight == 1) {
                    enemy.pos = secondNode.pos;
                }
            }
            playerMoves -= moveThreshold;
        }
        //increase score and energy if player is touching coin
        if(player1.pos == coin.pos) {
            coin.pos = randomLocation(map, white, whiteCells);
            score++;
            energy += coinEnergy;
        }
        //end game if player is touching enemies
        for(const auto& enemy : enemies) {
            if(player1.pos != enemy.pos) { continue; }
            printf("GAME OVER\n\n");
            FILE* hsStream = fopen("highscore", "r+");
            if(hsStream == NULL) {
                hsStream = fopen("highscore", "w");
                printf("Score: %d\n", score);
                assert(fwrite(&score, sizeof(score), 1, hsStream) == 1);
            } else {
                __typeof__(score) highscore;
                assert(fread(&highscore, sizeof(highscore), 1, hsStream) == 1);
                if(score > highscore) {
                    printf("New High Score!: %d\n", score);
                    fseek(hsStream, 0, SEEK_SET);
                    assert(fwrite(&score, sizeof(score), 1, hsStream) == 1);
                } else {
                    printf("Score: %d\nHigh Score: %d\n", score, highscore);
                }
            }
            fclose(hsStream);
            exit(0);
        }
        //get keys pressed
        numKeysPressed = 0;
        int nextKey = GetKeyPressed();
        while(nextKey && numKeysPressed < keysPressed.size()) {
            keysPressed[numKeysPressed] = nextKey;
            numKeysPressed++;
            nextKey = GetKeyPressed();
        }
        auto execTime = std::chrono::high_resolution_clock::now() - beginTime;
        BeginDrawing(); {
            
            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            
            //draw map
            for(unsigned y = 0; y < map.height; y++)
            for(unsigned x = 0; x < map.width; x++) {
                rgb pxl = rgbAt(map, x, y);
                DrawRectangle(
                    imgX + pxSize*x,
                    imgY + pxSize*y,
                    pxSize, pxSize,
                CLITERAL(Color){ pxl.r, pxl.g, pxl.b, 255 });
            }
            //draw player
            DrawEllipse(
                imgX + pxSize*player1.pos.x + pxSize/2,
                imgY + pxSize*player1.pos.y + pxSize/2,
                pxSize/2,
                pxSize/2,
                GREEN
            );
            //draw coin
            DrawEllipse(
                imgX + pxSize*coin.pos.x + pxSize/2,
                imgY + pxSize*coin.pos.y + pxSize/2,
                pxSize/2,
                pxSize/2,
                ORANGE
            );
            //draw enemies
            for(const auto& enemy : enemies) {
                int enemyX = imgX + pxSize*enemy.pos.x + pxSize/2;
                int enemyY = imgY + pxSize*enemy.pos.y + pxSize/2;
                DrawEllipse(enemyX, enemyY, pxSize/2, pxSize/2, RED);
                DrawText(TextFormat("%d", enemy.stunTimer), enemyX-10, enemyY-10, 20, WHITE);
            }
            //draw paths
            for(const auto& enemy : enemies) {
                // const auto& enemy = enemies[0];
                const auto& nodes = enemy.plannedPath.nodes;
                for(auto it = nodes.begin(); it != std::prev(nodes.end()); it++) {
                    const auto it2 = it+1;
                    DrawLine(
                        imgX + pxSize*it->pos.x + pxSize/2,
                        imgY + pxSize*it->pos.y + pxSize/2,
                        imgX + pxSize*it2->pos.x + pxSize/2,
                        imgY + pxSize*it2->pos.y + pxSize/2,
                        BLUE
                    );
                }
            }
            //draw score
            DrawText(TextFormat("SCORE: %d\n\n\nENERGY: %d", score, energy), 20, 200, 40, PINK);
            //draw time
            DrawText(TextFormat("exec time (ms): %lf", std::chrono::duration<double, std::milli>(execTime).count()), 10, 10, 20, LIME);

        } EndDrawing();
        //----------------------------------------------------------------------------------
    }
    free(map.pixels);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}