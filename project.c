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

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>

#include <errno.h>
#include <assert.h>

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb;
typedef struct {
    rgb* pixels;
    unsigned width;
    unsigned height;
} image;

#define rgbAt(_img, _x, _y) _img.pixels[_img.width*_y+_x]

int readUntilSpace(int fd, char* buf, int bufLen) {
    int i = 0;
    while(true) {
        if(i >= bufLen) {
            return -1;
        }
        read(fd, &buf[i], 1);
        if(isspace(buf[i])) {
            return i;
        }
        i++;
    }
}
//returns the first character that would be read from the stream
int skipWhitespaceAndComments(FILE* stream) {
    int nextChar;
    while(true) {
        nextChar = getc(stream);
        if(nextChar == '#') {
            do {
                nextChar = getc(stream);
            } while(nextChar != '\n');
        } else if(!isspace(nextChar)) {
            break;
        }
    }
    ungetc(nextChar, stream);
    return nextChar;
}
void perrorExit(const char* msg, int errorValue) {
    errno = errorValue;
    perror(msg);
    exit(-1);   
}
const char PPM_SIGNATURE[2] = "P6";
#define PPM_VALUE_MAX 255
int main(void) {
    //#define MAX_INT_STRING_LEN 10
    //#define BUF_LEN 512
    
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
    
    image img;
    //open ppm file
    {
        FILE* imgStream = fopen("img.ppm","rb");
        fseek(imgStream, 0, SEEK_END);
        long fileSize = ftell(imgStream);
        fseek(imgStream, 0, SEEK_SET);
        
        char signature[sizeof(PPM_SIGNATURE)];
        int amountRead = fread(signature, sizeof(signature), 1, imgStream);
        if(amountRead < 1 || memcmp(signature, PPM_SIGNATURE, sizeof(signature)) != 0) {
            perrorExit("malformed .ppm file - invalid signature", -1);
        }
        skipWhitespaceAndComments(imgStream);
        
        if(fscanf(imgStream, "%d", &img.width) != 1 || img.width <= 0) {
            perrorExit("malformed .ppm file - invalid width", -1);
        }
        skipWhitespaceAndComments(imgStream);
        
        if(fscanf(imgStream, "%d", &img.height) != 1 || img.height <= 0) {
            perrorExit("malformed .ppm file - invalid height", -1);
        }
        skipWhitespaceAndComments(imgStream);
        
        int maxValue;
        if(fscanf(imgStream, "%d", &maxValue) != 1 || maxValue != PPM_VALUE_MAX) {
            perrorExit("malformed .ppm file - invalid max colour value", -1);
        }
        
        if(getc(imgStream) != '\n') {
            perrorExit("malformed .ppm file - no newline between header and data", -1);   
        }
        
        long filePos = ftell(imgStream);
        long imgDataSize = (long)img.width*(long)img.height*3L;
        if(imgDataSize != fileSize-filePos) {
            perrorExit("malformed .ppm file - invalid data size", -1);   
        }
        
        img.pixels = malloc(imgDataSize);
        amountRead = fread(img.pixels, imgDataSize, 1, imgStream);
        //sanity check
        if(amountRead < 1 || ftell(imgStream) != fileSize || getc(imgStream) != EOF) {
            perrorExit("error occured while reading data from .ppm file", -1);
        }
        assert(fclose(imgStream) == 0);
        printf("\\n: %d, \\r: %d", '\n', '\r');
    }
    
    
    int startX;
    int startY;
    int endX;
    int endY;
    

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