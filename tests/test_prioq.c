#include "acutest.h" //use -E -v (exec and verbose)
#include "collections/prioq.h"

#include <stdbool.h>

#define MY_TC(x) TEST_CHECK_(x, "%-50s", #x);
#define tlitem(x) {#x"()", x}

void test_length() {
    prioq* queue = new_prioq();
    MY_TC(queue != NULL);
    MY_TC(0 == queue->len                        );
    MY_TC(prioq_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(1 == queue->len                        );
    MY_TC(prioq_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(2 == queue->len);
    MY_TC(prioq_add(queue, (prioqElt) {}, 0) == 0);
    MY_TC(3 == queue->len                        );
    delete_prioq(queue);
}
void test_peekAndPoll() {
    prioq* queue = new_prioq();
    prioqElt ret;
    prioqElt* cmpLitPtr;
    cmpLitPtr = &(prioqElt) {12, 34};

    MY_TC(prioq_add(queue, (prioqElt) {12, 34}, 0) == 0);
    ret = prioq_peek(queue); cmpLitPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {56, 78}, 0) == 0);
    ret = prioq_peek(queue); cmpLitPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {-9, 10}, 0) == 0);
    ret = prioq_peek(queue); cmpLitPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {12, 34}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_peek(queue); cmpLitPtr = &(prioqElt) {56, 78}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {56, 78}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_peek(queue); cmpLitPtr = &(prioqElt) {-9, 10}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {-9, 10}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    MY_TC(queue->len == 0);
    delete_prioq(queue);
}
void test_priority() {
    prioq* queue = new_prioq();
    prioqElt ret;
    prioqElt* cmpLitPtr;
    MY_TC(prioq_add(queue, (prioqElt) {1, 0}, 1) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {2, 0}, 1) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {3, 0}, 1) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {4, 0}, 1) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {5, 0}, 2) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {6, 0}, 2) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {7, 0}, 2) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {8, 0}, 0) == 0);
    MY_TC(prioq_add(queue, (prioqElt) {9, 0}, 0) == 0);
    MY_TC(queue->len == 9);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {5, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {6, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {7, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {1, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {2, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {3, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {4, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {8, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    ret = prioq_poll(queue); cmpLitPtr = &(prioqElt) {9, 0}; MY_TC(memcmp(&ret, cmpLitPtr, sizeof(prioqElt)) == 0);
    MY_TC(queue->len == 0);
    delete_prioq(queue);
}
TEST_LIST = {
    tlitem(test_length),
    tlitem(test_peekAndPoll),
    tlitem(test_priority),
{NULL, NULL}};
