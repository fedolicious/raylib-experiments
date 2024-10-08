#include "test_main.h"
#define TEST_NO_MAIN
#include "acutest.h"

#include "collections/prioq.h"

#include <stdbool.h>

struct prioqElt {
    int x;
    int y;
    int priority;
    bool operator==(const prioqElt& other) const {
        return
            x == other.x &&
            y == other.y &&
            priority == other.priority;
    }
};
inline void delete_testType(prioqElt) { return; }

DEFINE_PRIOQ(prioqElt, testType)
int cmpPQE(prioqElt A, prioqElt B) {
    return -(A.priority - B.priority);
}

void test_prioq_length() {
    prioq_testType* queue = new_prioq_testType(cmpPQE);
    
    MY_TC(queue != NULL);
    MY_TC(0 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}) == 0);
    MY_TC(1 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}) == 0);
    MY_TC(2 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}) == 0);
    MY_TC(3 == queue->len);
    
    delete_prioq_testType(queue);
}
void test_prioq_peekAndPoll() {
    prioq_testType* queue = new_prioq_testType(cmpPQE);

    MY_TC(prioq_testType_add(queue, (prioqElt){12, 34, 0}) == 0);
    MY_TC(prioq_testType_peek(queue) == ((prioqElt){12, 34, 0}));
    MY_TC(prioq_testType_add(queue, (prioqElt){56, 78, 0}) == 0);
    MY_TC(prioq_testType_peek(queue) == ((prioqElt){12, 34, 0}));
    MY_TC(prioq_testType_add(queue, (prioqElt){-9, 10, 0}) == 0);
    MY_TC(prioq_testType_peek(queue) == ((prioqElt){12, 34, 0}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){12, 34, 0}));
    MY_TC(prioq_testType_peek(queue) == ((prioqElt){56, 78, 0}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){56, 78, 0}));
    MY_TC(prioq_testType_peek(queue) == ((prioqElt){-9, 10, 0}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){-9, 10, 0}));
    MY_TC(queue->len == 0);
    
    delete_prioq_testType(queue);
}
void test_prioq_priority() {
    prioq_testType* queue = new_prioq_testType(cmpPQE);
    
    MY_TC(prioq_testType_add(queue, (prioqElt){1, 0, 1}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){2, 0, 1}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){3, 0, 1}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){4, 0, 1}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){5, 0, 2}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){6, 0, 2}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){7, 0, 2}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){8, 0, 0}) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt){9, 0, 0}) == 0);
    MY_TC(queue->len == 9);
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){5, 0, 2}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){6, 0, 2}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){7, 0, 2}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){1, 0, 1}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){2, 0, 1}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){3, 0, 1}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){4, 0, 1}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){8, 0, 0}));
    MY_TC(prioq_testType_poll(queue) == ((prioqElt){9, 0, 0}));
    MY_TC(queue->len == 0);
    
    delete_prioq_testType(queue);
}
