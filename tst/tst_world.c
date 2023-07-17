#include "include/tst_world.h"
#include "world.h"

struct test tests_world[] = {
    {tst_world_init, "world_init"},
    {tst_world_get, "world_get"},
    {tst_world_set, "world_set"},
    {tst_world_get_sort, "world_get_sort"},
    {tst_world_set_sort, "world_set_sort"},
};

tests tst_world_list() {
    return create_tests(tests_world, TST_COUNT_WORLD);
}

void tst_world_init() {
    struct world_t* w = world_init();

    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        assert(world_get(w, i) == NO_COLOR);
        assert(world_get_sort(w, i) == NO_SORT);
    }
}

void tst_world_get() {
    struct world_t* w = world_init();

    for (int i = 0; i < WIDTH * HEIGHT; i += 3)
        world_set(w, i, WHITE);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (!(i % 3))
            assert(world_get(w, i) == WHITE);
        else
            assert(world_get(w, i) == NO_COLOR);
    }
}

void tst_world_set() {
    struct world_t* w = world_init();

    for (int i = 0; i < WIDTH * HEIGHT; i += 3)
        world_set(w, i, WHITE);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (!(i % 3))
            assert(world_get(w, i) == WHITE);
        else
            assert(world_get(w, i) == NO_COLOR);
    }
}

void tst_world_get_sort() {
    struct world_t* w = world_init();

    for (int i = 0; i < WIDTH * HEIGHT; i += 3)
        world_set_sort(w, i, PAWN);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (!(i % 3))
            assert(world_get_sort(w, i) == PAWN);
        else
            assert(world_get_sort(w, i) == NO_SORT);
    }
}

void tst_world_set_sort() {
    struct world_t* w = world_init();

    for (int i = 0; i < WIDTH * HEIGHT; i += 3)
        world_set_sort(w, i, PAWN);
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        if (!(i % 3))
            assert(world_get_sort(w, i) == PAWN);
        else
            assert(world_get_sort(w, i) == NO_SORT);
    }
}
