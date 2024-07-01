#include "acutest.h" //use -E -v (exec and verbose)
#include "collections/prioq.h"

#include <stdbool.h>

#define MY_TC(x) TEST_CHECK_(x, "%-50s", #x);


typedef struct {
    int x;
    int y;
} prioqElt;
inline void delete_testType(prioqElt) { return; }

DEFINE_PRIOQ(prioqElt, testType)

void test_length() {
    prioq_testType* queue = new_prioq_testType();
    MY_TC(queue != NULL);
    MY_TC(0 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(1 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(2 == queue->len);
    MY_TC(prioq_testType_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(3 == queue->len);
    delete_prioq_testType(queue);
}
void test_peekAndPoll() {
    prioq_testType* queue = new_prioq_testType();
    prioqElt ret;
    prioqElt* litPtr;

    MY_TC(prioq_testType_add(queue, (prioqElt) {12, 34}, 0) == 0);
    ret = prioq_testType_peek(queue); litPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {56, 78}, 0) == 0);
    ret = prioq_testType_peek(queue); litPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {-9, 10}, 0) == 0);
    ret = prioq_testType_peek(queue); litPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_peek(queue); litPtr = &(prioqElt) {56, 78}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {56, 78}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_peek(queue); litPtr = &(prioqElt) {-9, 10}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {-9, 10}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    MY_TC(queue->len == 0);
    delete_prioq_testType(queue);
}
void test_priority() {
    prioq_testType* queue = new_prioq_testType();
    prioqElt ret;
    prioqElt* litPtr;
    MY_TC(prioq_testType_add(queue, (prioqElt) {1, 0}, 1) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {2, 0}, 1) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {3, 0}, 1) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {4, 0}, 1) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {5, 0}, 2) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {6, 0}, 2) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {7, 0}, 2) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {8, 0}, 0) == 0);
    MY_TC(prioq_testType_add(queue, (prioqElt) {9, 0}, 0) == 0);
    MY_TC(queue->len == 9);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {5, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {6, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {7, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {1, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {2, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {3, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {4, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {8, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    ret = prioq_testType_poll(queue); litPtr = &(prioqElt) {9, 0}; MY_TC(memcmp(&ret, litPtr, sizeof(prioqElt)) == 0);
    MY_TC(queue->len == 0);
    delete_prioq_testType(queue);
}

#define tlitem(x) {#x"()", x}
TEST_LIST = {
    tlitem(test_length),
    tlitem(test_peekAndPoll),
    tlitem(test_priority),
{NULL, NULL}};
