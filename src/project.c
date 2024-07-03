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
#include "collections/prioq.h"
#include "mystdlib.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

typedef struct {
    int x;
    int y;
    int gVal;
    int cameFrom;
    bool isWall;
    bool isInPrioq;
} aStarPt;
//cleanup is handled by the graph, not the priority queue
inline void delete_asp(aStarPt*) { return; }
typedef struct {
    int x;
    int y;
} ivec2;
ivec2 neighborOffsets[4] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1},
};
DEFINE_PRIOQ(aStarPt*, asp);
static unsigned startX;
static unsigned startY;
static unsigned endX;
static unsigned endY;
int cmpAStarNode(aStarPt* A, aStarPt* B) {
    const int AhVal = abs(A->x-endX)+abs(A->y-endY);
    const int BhVal = abs(B->x-endX)+abs(B->y-endY);
    const int AfVal = A->gVal + AhVal;
    const int BfVal = B->gVal + BhVal;
    return AfVal - BfVal;
}
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

    //a star
    {
        startX = 0;
        startY = 0;
        endX = img.width-1;
        endY = img.height-1;
        aStarPt* graph[img.width*img.height];
        for(int y = 0; y < img.height; y++) {
            for(int x = 0; x < img.width; x++) {
                graph[img.width*y + x] = malloc(sizeof(aStarPt));
                *graph[img.width*y + x] = (aStarPt){
                    .x = x,
                    .y = y,
                    .gVal = INT_MAX,
                    .isWall = (rgbAt(img, x, y).r == 0),
                    .isInPrioq = false,
                };
            }
        }
        prioq_asp* nodes = new_prioq_asp(cmpAStarNode);

        //add first point
        aStarPt* start = graph[img.width*startY + startX];
        if(start->isWall) { perrorExit("A* error - Start location is a wall", -1); }
        if(graph[img.width*endY + endX]->isWall) { perrorExit("A* error - End location is a wall", -1); }
        start->gVal = 0;
        start->cameFrom = -1;
        start->isInPrioq = true;
        prioq_asp_add(nodes, start);
        bool goalFound = false;
        aStarPt* bestNode;
        aStarPt* fakeNode = malloc(sizeof(aStarPt));
        *fakeNode = (aStarPt){
            .x = endX,
            .y = endY,
            .gVal = 0,
        };
        while(nodes->len > 0) {
            bestNode = prioq_asp_poll(nodes);
            bestNode->isInPrioq = false;
            if(bestNode->x == endX && bestNode->y == endY) {
                goalFound = true;
                break;
                log("\nGOAL FOUND!! distance: %d\n\n",bestNode->gVal); exit(1);
                //GOAL FOUND (return)
            }
            //check neighbors
            for(int i = 0; i < arrlen(neighborOffsets); i++) {
                const int neighborX = bestNode->x + neighborOffsets[i].x;
                if(!(0 <= neighborX && neighborX < img.width)) { continue; }
                const int neighborY = bestNode->y + neighborOffsets[i].y;
                if(!(0 <= neighborY && neighborY < img.height)) { continue; }
                aStarPt* neighbor = graph[img.width*neighborY + neighborX];
                
                
                
                if(neighbor->isWall || neighbor->gVal <= bestNode->gVal+1) { continue; }
                neighbor->gVal = bestNode->gVal+1;              
                neighbor->cameFrom = img.width*bestNode->y + bestNode-> x;                                
                if(!neighbor->isInPrioq) {
                    prioq_asp_add(nodes, neighbor);
                    //TODO RECALCULATE neighbor'S POSITION IN THE PRIOQ IF IT IS IMPLEMENTED AS A HEAP     
                    neighbor->isInPrioq = true;
                }
            }
        }
        if(!goalFound) {
            log("\nFAILED TO FIND GOAL\n\n"); exit(2);
        } else {
            aStarPt* node = bestNode;
            while(node->cameFrom != -1) {
                rgbAt(img, node->x, node->y).g = 64;
                node = graph[node->cameFrom];
            }
        }
        delete_prioq_asp(nodes);
        for(int i = 0; i < arrlen(graph); i++) {
            free(graph[i]);
        }
    }
    int imgX = 10;
    int imgY = 10;
    int pxWidth = 10;
    int pxHeight = 10;
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
            
            //draw ppm
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
            if(IsKeyPressed(KEY_UP)) {imgY--;}
            if(IsKeyPressed(KEY_DOWN)) {imgY++;}
            if(IsKeyPressed(KEY_LEFT)) {imgX--;}
            if(IsKeyPressed(KEY_RIGHT)) {imgX++;}
            if(IsKeyPressed(KEY_EQUAL)) {pxWidth++; pxHeight++;}
            if(IsKeyPressed(KEY_MINUS)) {pxWidth--; pxHeight--;}
            //show keys pressed
            /*{
                int numKeysPressed = 0;
                int keys[10];
                int nextKey = GetKeyPressed();
                while(nextKey && numKeysPressed < arrlen(keys)) {
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