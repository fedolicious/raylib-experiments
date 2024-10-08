#pragma once

#define MY_TC(x) TEST_CHECK_(x, "%-50s", #x);

#define FUNC_LIST(f) \
    f(test_prioq_length)      \
    f(test_prioq_peekAndPoll) \
    f(test_prioq_priority)    \
    f(test_astar_maze1)       \
    f(test_astar_maze2)       \
    f(test_astar_maze3)       \
    f(test_astar_mazeTripleWeight) \
    f(test_astar_mazeNoPath) \
    f(test_astar_mazeStartInWall) \
    f(test_astar_mazeGoalInWall) \
    f(test_astar_mazeStartAndGoalInWall) \
    f(test_astar_startInGoal) \
    f(test_graph_loops) \
    f(test_graph_blank) \

#define VOID_FUNC(x) void x();

FUNC_LIST(VOID_FUNC)