#include "pathfind.h"

#include "collections/prioq.h"

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    int x;
    int y;
    int gVal;
    int cameFrom;
    bool isWall;
    bool isInPrioq;
} aStarNode;
//cleanup is handled by the graph, not the priority queue
inline void delete_asn(aStarNode*) { return; }
DEFINE_PRIOQ(aStarNode*, asn)

ivec2 neighborOffsets[4] = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1},
};
static ivec2 stcGoal;
int cmpAStarNode(aStarNode* A, aStarNode* B) {
    const int AhVal = abs(A->x-stcGoal.x)+abs(A->y-stcGoal.y);
    const int BhVal = abs(B->x-stcGoal.x)+abs(B->y-stcGoal.y);
    const int AfVal = A->gVal + AhVal;
    const int BfVal = B->gVal + BhVal;
    return AfVal - BfVal;
}

path aStar(const image navImg, const ivec2 start, const ivec2 goal) {
    assert(0 <= start.x && start.x < navImg.width);
    assert(0 <= start.y && start.y < navImg.height);
    assert(0 <= goal.x && goal.x < navImg.width);
    assert(0 <= goal.y && goal.y < navImg.height);
    assert(INT_MAX/navImg.width >= navImg.height);
    aStarNode* graph[navImg.width*navImg.height];
    for(int y = 0; y < navImg.height; y++) {
        for(int x = 0; x < navImg.width; x++) {
            graph[navImg.width*y + x] = malloc(sizeof(aStarNode));
            *graph[navImg.width*y + x] = (aStarNode){
                .x = x,
                .y = y,
                .gVal = INT_MAX,
                .isWall = (rgbAt(navImg, x, y).r == 0),
                .isInPrioq = false,
            };
        }
    }
    stcGoal.x = goal.x;
    stcGoal.y = goal.y;
    prioq_asn* nodes = new_prioq_asn(cmpAStarNode);

    //add first point
    aStarNode* startNode = graph[navImg.width*start.y + start.x];
    if(startNode->isWall) { perrorExit("A* error - Start location is a wall", -1); }
    if(graph[navImg.width*goal.y + goal.x]->isWall) { perrorExit("A* error - End location is a wall", -1); }
    startNode->gVal = 0;
    startNode->cameFrom = -1;
    startNode->isInPrioq = true;
    prioq_asn_add(nodes, startNode);
    bool goalFound = false;
    aStarNode* bestNode;
    while(nodes->len > 0) {
        bestNode = prioq_asn_poll(nodes);
        bestNode->isInPrioq = false;
        if(bestNode->x == goal.x && bestNode->y == goal.y) {
            goalFound = true;
            break;
        }
        //check neighbors
        for(int i = 0; i < arrlen(neighborOffsets); i++) {
            const int neighborX = bestNode->x + neighborOffsets[i].x;
            if(!(0 <= neighborX && neighborX < navImg.width)) { continue; }
            const int neighborY = bestNode->y + neighborOffsets[i].y;
            if(!(0 <= neighborY && neighborY < navImg.height)) { continue; }
            aStarNode* neighbor = graph[navImg.width*neighborY + neighborX];
            
            if(neighbor->isWall || neighbor->gVal <= bestNode->gVal+1) { continue; }
            neighbor->gVal = bestNode->gVal+1;              
            neighbor->cameFrom = navImg.width*bestNode->y + bestNode-> x;                                
            if(!neighbor->isInPrioq) {
                prioq_asn_add(nodes, neighbor);
                //TODO RECALCULATE neighbor'S POSITION IN THE PRIOQ IF IT IS IMPLEMENTED AS A HEAP     
                neighbor->isInPrioq = true;
            }
        }
    }
    path ret;
    if(!goalFound) {
        ret = (path) { .points = NULL, .len = 0, };
    } else {
        const int pointCount = bestNode->gVal+1;
        ret = (path) {
            .points = malloc(pointCount * sizeof(*ret.points)),
            .len = pointCount,
        };
        aStarNode* node = bestNode;
        for(int i = ret.len-1; i > 0; i--) {
            ret.points[i].x = node->x;
            ret.points[i].y = node->y;
            node = graph[node->cameFrom];
        }
        ret.points[0].x = node->x;
        ret.points[0].y = node->y;
        assert(node->cameFrom == -1);
    }
    delete_prioq_asn(nodes);
    for(int i = 0; i < arrlen(graph); i++) {
        free(graph[i]);
    }
    return ret;
}
    