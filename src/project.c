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
typedef struct {
    ivec2 pos;
} enemy;

int main(void) {
    const rgb white = (rgb){255,255,255};
    const rgb black = (rgb){0,0,0};
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    SetTargetFPS(60);
    
    chdir("resources");
    
    FILE* logStream = fopen("log.txt", "w");
    
    image map = imgio_readPPM("map.ppm");
    
    player player1;
    player1 = (player) {
        .pos.x = 46,
        .pos.y = 22,
    };
    enemy enemyArr[2] = {
        {.pos.x = 10, .pos.y = 27},
        {.pos.x = 20, .pos.y = 1},
    };
    
    if(memcmp(&rgbAt(map, player1.pos.x, player1.pos.y), &white, sizeof(rgb)) != 0) {
        perrorExit("Player starting location location must be open", -1);
    }
    
    int imgX = 10;
    int imgY = 10;
    int pxWidth = 15;
    int pxHeight = 15;
    int keysPressed[10];
    int numKeysPressed;
    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
            //zoom in/out
            if(IsKeyPressed(KEY_EQUAL)) {pxWidth++; pxHeight++;}
            if(IsKeyPressed(KEY_MINUS)) {pxWidth--; pxHeight--;}
            
            //move player
            ivec2 contenderPos = player1.pos;
            if(IsKeyPressed(KEY_UP)) {contenderPos.y--;}
            if(IsKeyPressed(KEY_DOWN)) {contenderPos.y++;}
            if(IsKeyPressed(KEY_LEFT)) {contenderPos.x--;}
            if(IsKeyPressed(KEY_RIGHT)) {contenderPos.x++;}
            if(memcmp(&rgbAt(map, contenderPos.x, contenderPos.y), &white, sizeof(rgb)) == 0) {
                player1.pos = contenderPos;
            } else {
                //failed to move
            }
            //get enemy paths
            path enemyPaths[arrlen(enemyArr)];
            for(int i = 0; i < arrlen(enemyArr); i++) {
                //block off nodes where other enemies are
                for(int j = 0; j < arrlen(enemyArr); j++) {
                    if(j != i) {
                        rgbAt(map, enemyArr[j].pos.x, enemyArr[j].pos.y) = black;
                    }
                }
                //pathfind
                enemyPaths[i] = aStar(map, enemyArr[i].pos, player1.pos);
                //unblock nodes
                for(int i = 0; i < arrlen(enemyArr); i++) {
                    rgbAt(map, enemyArr[i].pos.x, enemyArr[i].pos.y) = white;
                }
            }
            //move enemies along paths
            if(IsKeyPressed(KEY_SPACE)) {
                for(int i = 0; i < arrlen(enemyArr); i++) {
                    if(enemyPaths[i].len <= 2) { continue; }
                    assert(memcmp(&enemyPaths[i].points[0], &enemyArr[i].pos, sizeof(enemyArr[i].pos)) == 0);
                    assert(taxicabDist(enemyPaths[i].points[1], enemyArr[i].pos) == 1);
                    enemyArr[i].pos = enemyPaths[i].points[1];
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
                    imgX + pxWidth*x,
                    imgY + pxHeight*y,
                    pxWidth, pxHeight,
                CLITERAL(Color){ pxl.r, pxl.g, pxl.b, 255 });
            }
            //draw player
            DrawEllipse(
                imgX + pxWidth*player1.pos.x + pxWidth/2,
                imgY + pxHeight*player1.pos.y + pxHeight/2,
                pxWidth/2,
                pxHeight/2,
                GREEN
            );
            //draw enemies
            for(int i = 0; i < arrlen(enemyArr); i++) {
                DrawEllipse(
                    imgX + pxWidth*enemyArr[i].pos.x + pxWidth/2,
                    imgY + pxHeight*enemyArr[i].pos.y + pxHeight/2,
                pxWidth/2, pxHeight/2, RED);
            }
            //draw paths
            for(int j = 0; j < arrlen(enemyPaths); j++)
            for(int i = 0; i < enemyPaths[j].len-1; i++) {
                const ivec2* pts = enemyPaths[j].points;
                DrawLine(
                    imgX + pxWidth*pts[i].x + pxWidth/2,
                    imgY + pxHeight*pts[i].y + pxHeight/2,
                    imgX + pxWidth*pts[i+1].x + pxWidth/2,
                    imgY + pxHeight*pts[i+1].y + pxHeight/2,
                BLUE);
            }
            
            //show keys pressed
            for(int i = 0; i < numKeysPressed; i++) {
                DrawText(TextFormat("%d", keysPressed[i]), 20, 200 + 20*i, 15, BLACK);
            }

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