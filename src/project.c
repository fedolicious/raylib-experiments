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

int main(void) {
    chdir("resources");
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    FILE* logStream = fopen("log.txt", "w");
    
    const int len = 10;
    char* str = malloc(len * sizeof(char));
    for(int i = 0; i < len-1; i++) {
        str[i] = 'a'+i;
    }
    str[len-1] = '\0';
    
    image img = imgio_readPPM("img.ppm");
    
    path p = aStar(img, (ivec2){0, 0}, (ivec2){img.width-1, img.height-1});
    for(int i = 0; i < p.len; i++) {
        rgbAt(img, p.points[i].x, p.points[i].y).g = 64;
    }
    if(p.points != NULL) { free(p.points); }
    int imgX = 10;
    int imgY = 10;
    int pxWidth = 10;
    int pxHeight = 10;
    int keysPressed[10];
    int numKeysPressed;
    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        {
            str[0]++;
            if(str[0] > 'z') { str[0] = 'a'; }
            
            if(IsKeyPressed(KEY_UP)) {imgY--;}
            if(IsKeyPressed(KEY_DOWN)) {imgY++;}
            if(IsKeyPressed(KEY_LEFT)) {imgX--;}
            if(IsKeyPressed(KEY_RIGHT)) {imgX++;}
            if(IsKeyPressed(KEY_EQUAL)) {pxWidth++; pxHeight++;}
            if(IsKeyPressed(KEY_MINUS)) {pxWidth--; pxHeight--;}
            
            numKeysPressed = 0;
            int nextKey = GetKeyPressed();
            while(nextKey && numKeysPressed < arrlen(keysPressed)) {
                keysPressed[numKeysPressed] = nextKey;
                numKeysPressed++;
                nextKey = GetKeyPressed();
            }
        }
        BeginDrawing(); {

            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            DrawText(str, 190, 240, 20, BLACK);
            
            //draw ppm
            for(int y = 0; y < img.height; y++)
            for(int x = 0; x < img.width; x++) {
                rgb pxl = rgbAt(img, x, y);
                DrawRectangle(
                    imgX + pxWidth*x,
                    imgY + pxHeight*y,
                    pxWidth, pxHeight,
                CLITERAL(Color){ pxl.r, pxl.g, pxl.b, 255 });
            }
            
            //show keys pressed
            for(int i = 0; i < numKeysPressed; i++) {
                DrawText(TextFormat("%d", keysPressed[i]), 20, 200 + 20*i, 15, BLACK);
            }

        } EndDrawing();
        //----------------------------------------------------------------------------------
    }
    assert(fclose(logStream) == 0);
    free(img.pixels);
    free(str);

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}