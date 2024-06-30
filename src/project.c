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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>


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
        
    int startX = 0;
    int startY = 0;
    int endX = img.width-1;
    int endY = img.height-1;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        str[0]++;
        if(str[0] > 'z') { str[0] = 'a'; }
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            DrawText(str, 190, 240, 20, BLACK);
            const int imgX = 10;
            const int imgY = 10;
            const int pxWidth = 10;
            const int pxHeight = 10;
            for(int y = 0; y < img.height; y++) {
                for(int x = 0; x < img.width; x++) {
                    rgb pxl = rgbAt(img, x, y);
                    DrawRectangle(
                        imgX + pxWidth*x,
                        imgY + pxHeight*y,
                        pxWidth, pxHeight,
                    CLITERAL(Color){ pxl.r, pxl.g, pxl.b, 255 });
                }
            }
            /*{
                int numKeysPressed = 0;
                int keys[10];
                int nextKey = GetKeyPressed();
                while(nextKey && numKeysPressed < sizeof(keys)/sizeof(*keys)) {
                    keys[numKeysPressed] = nextKey;
                    numKeysPressed++;
                    nextKey = GetKeyPressed();
                }
                for(int i = 0; i < numKeysPressed; i++) {
                    DrawText(TextFormat("%d", keys[i]), 20, 200 + 20*i, 15, BLACK);
                }
            }*/

        EndDrawing();
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