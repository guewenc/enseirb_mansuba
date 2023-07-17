#include "include/tst_ens.h"
#include <limits.h>
#include "ens.h"

struct test tests_ens[] = {
    {tst_ens_create, "ens_create"},
    {tst_ens_found_idx, "ens_found_idx"},
    {tst_ens_set_val, "ens_set_val"},
    {tst_ens_set_sort, "ens_set_sort"},
    {tst_ens_set_color, "ens_set_color"},
    {tst_ens_get_val, "ens_get_val"},
    {tst_ens_get_sort, "ens_get_sort"},
    {tst_ens_get_color, "ens_get_color"}};

tests tst_ens_list() {
    return create_tests(tests_ens, TST_COUNT_ENS);
}

void tst_ens_create() {
    struct ens e_0 = ens_create(0);
    struct ens e_42 = ens_create(42);
    for (int i = 0; i < WORLD_SIZE; i++) {
        assert(ens_get_val(e_0, i) == 0);
        assert(ens_get_val(e_42, i) == 42);
    }
}

void tst_ens_found_idx() {
    struct ens e = ens_create(0);

    ens_set_val(&e, 0, 42);

    assert(ens_found_idx(e, 42, WORLD_SIZE) == 0);
    assert(ens_found_idx(e, 150, WORLD_SIZE) == UINT_MAX);
}

void tst_ens_set_val() {
    struct ens e = ens_create(0);
    ens_set_val(&e, 0, 42);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 0)
            assert(ens_get_val(e, i) == 42);
        else
            assert(ens_get_val(e, i) == 0);
    }
}

void tst_ens_set_sort() {
    struct ens e = ens_create(NO_SORT);
    ens_set_sort(&e, 0, PAWN);
    ens_set_sort(&e, 16, TOWER);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 0)
            assert(ens_get_sort(e, i) == PAWN);
        else if (i == 16)
            assert(ens_get_sort(e, i) == TOWER);
        else
            assert(ens_get_sort(e, i) == NO_SORT);
    }
}

void tst_ens_set_color() {
    struct ens e = ens_create(NO_COLOR);
    ens_set_color(&e, 0, WHITE);
    ens_set_color(&e, 16, BLACK);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 0)
            assert(ens_get_color(e, i) == WHITE);
        else if (i == 16)
            assert(ens_get_color(e, i) == BLACK);
        else
            assert(ens_get_color(e, i) == NO_COLOR);
    }
}

void tst_ens_get_val() {
    struct ens e = ens_create(0);
    ens_set_val(&e, 0, 42);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 0)
            assert(ens_get_val(e, i) == 42);
        else
            assert(ens_get_val(e, i) == 0);
    }
}

void tst_ens_get_sort() {
    struct ens e = ens_create(NO_SORT);
    ens_set_sort(&e, 12, PAWN);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 12)
            assert(ens_get_sort(e, i) == PAWN);
        else
            assert(ens_get_sort(e, i) == NO_SORT);
    }
}

void tst_ens_get_color() {
    struct ens e = ens_create(NO_COLOR);
    ens_set_color(&e, 9, WHITE);

    for (int i = 0; i < WORLD_SIZE; i++) {
        if (i == 9)
            assert(ens_get_color(e, i) == WHITE);
        else
            assert(ens_get_color(e, i) == NO_COLOR);
    }
}
