#include "include/utils.h"
#include <stdbool.h>

tests create_tests(struct test* test, int size) {
    return (tests){.list = test, .test_count = size};
}