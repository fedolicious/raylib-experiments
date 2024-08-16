#include "test_main.h"

#include "acutest.h" //use -E -v (exec and verbose)

#define TLITEM(x) {#x"()", x},
TEST_LIST = {
    FUNC_LIST(TLITEM)
{NULL, NULL}};