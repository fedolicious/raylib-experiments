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
#include "mystdlib.h"
#include "nav/pathfind.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

#define log(...) fprintf(logStream, __VA_ARGS__); fflush(logStream)
typedef struct {
    ivec2 pos;
} player;
struct enemy {
    ivec2 pos;
    int stunTimer;
    enemy(ivec2 p) :
        pos{p},
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
                .x = i%graph.width,
                .y = i/graph.width,
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
    
    chdir("resources");
    
    FILE* logStream = fopen("log.txt", "w");
    
    image map = imgio_readPPM("map.ppm");
    int whiteCells = 0;
    for(unsigned i = 0; i < map.width*map.height; i++) {
        if(memcmp(&map.pixels[i], &white, sizeof(rgb)) == 0) {
            whiteCells++;
        }
    }
    assert(whiteCells > 0);
    player player1{ivec2{46, 22}};
    enemy enemies[2] = {
        ivec2{10, 27},
        ivec2{20, 1},
        //ivec2{3, 12},
    };
    
    if(memcmp(&rgbAt(map, player1.pos.x, player1.pos.y), &white, sizeof(rgb)) != 0) {
        perrorExit("Player starting location location must be open", -1);
    }
    
    int imgX = 10;
    int imgY = 10;
    int pxSize = 35;
    
    int keysPressed[10];
    int numKeysPressed;
    
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
        //calculate each path to the player
        path enemyPaths[arrlen(enemies)];
        int enemyIndxs[arrlen(enemies)];
        for(int i = 0; i < arrlen(enemyPaths); i++) {
            enemyPaths[i] = aStar(map, enemies[i].pos, player1.pos, isNotWhite);
            enemyIndxs[i] = i;
        }
        //sort (bubble)
        for(int j = 0; j < arrlen(enemyPaths)-1; j++)
        for(int i = 0; i < arrlen(enemyPaths)-1; i++) {
            if(enemyPaths[i].len > enemyPaths[i+1].len) {
                memswap(&enemyPaths[i], &enemyPaths[i+1], sizeof(*enemyPaths));
                memswap(&enemyIndxs[i], &enemyIndxs[i+1], sizeof(*enemyIndxs));
            }
        }
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
                enemyPaths[i+1] = aStar(map, enemies[enemyIndxs[i+1]].pos, player1.pos, isNotWhite);
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
        //move one step in the game
        if(IsKeyPressed(KEY_SPACE) || playerMoves >= moveThreshold) {
            //decay all walls
            for(int i = 0; i < map.width*map.height; i++) {
                rgb* pixel = &map.pixels[i];
                if(pixel->g == 255 && pixel->r < 255) {
                    unsigned char newR = pixel->r + 60;
                    pixel->r = (newR < pixel->r)? 255 : newR;
                }
            }
            //move enemies along paths
            for(int i = 0; i < arrlen(enemies); i++) {
                if(!enemyPaths[i].isValid() || enemyPaths[i].len < 2) { continue; }
                if(enemies[enemyIndxs[i]].stunTimer > 0) {
                    enemies[enemyIndxs[i]].stunTimer --;
                    continue;
                }
                enemy* const currEnemy = &enemies[enemyIndxs[i]];
                assert(memcmp(&enemyPaths[i].points[0], &currEnemy->pos, sizeof(currEnemy->pos)) == 0);
                assert(taxicabDist(enemyPaths[i].points[1], currEnemy->pos) == 1);
                currEnemy->pos = enemyPaths[i].points[1];
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
        for(int i = 0; i < arrlen(enemies); i++) {
            if(player1.pos == enemies[i].pos) {
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
        }
        //get keys pressed
        numKeysPressed = 0;
        int nextKey = GetKeyPressed();
        while(nextKey && numKeysPressed < arrlen(keysPressed)) {
            keysPressed[numKeysPressed] = nextKey;
            numKeysPressed++;
            nextKey = GetKeyPressed();
        }
        
        BeginDrawing(); {

            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            
            //draw map
            for(int y = 0; y < map.height; y++)
            for(int x = 0; x < map.width; x++) {
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
            for(int i = 0; i < arrlen(enemies); i++) {
                int enemyX = imgX + pxSize*enemies[i].pos.x + pxSize/2;
                int enemyY = imgY + pxSize*enemies[i].pos.y + pxSize/2;
                DrawEllipse(enemyX, enemyY, pxSize/2, pxSize/2, RED);
                DrawText(TextFormat("%d", enemies[i].stunTimer), enemyX-10, enemyY-10, 20, WHITE);
            }
            //draw score
            DrawText(TextFormat("SCORE: %d\n\n\nENERGY: %d", score, energy), 20, 200, 40, PINK);

        } EndDrawing();
        
        for(int i = 0; i < arrlen(enemyPaths); i++) {
            if(enemyPaths[i].points != NULL) { free(enemyPaths[i].points); }
        }
        //----------------------------------------------------------------------------------
    }
    assert(fclose(logStream) == 0);
    free(map.pixels);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}