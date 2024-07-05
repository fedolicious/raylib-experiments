#pragma once

#define MY_TC(x) TEST_CHECK_(x, "%-50s", #x);

#define FUNC_LIST(f) \
    f(test_prioq_length)      \
    f(test_prioq_peekAndPoll) \
    f(test_prioq_priority)    \
    f(test_astar_maze1)       \
    f(test_astar_maze2)       \
    f(test_astar_maze3)       \
    f(test_astar_maze4)       \

#define VOID_FUNC(x) void x();

FUNC_LIST(VOID_FUNC)