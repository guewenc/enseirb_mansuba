#ifndef __TST_UTILS_H__
#define __TST_UTILS_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG
#define TESTS_CAT_COUNT 6

#ifndef DEBUG
#define TRACE(format, ...)
#else
#define TRACE(format, ...) printf(format, __VA_ARGS__)
#endif

struct test {
    void (*f)();
    char* test_name;
};

typedef struct tests {
    struct test* list;
    int test_count;
} tests;

tests create_tests(struct test* test, int size);

#endif