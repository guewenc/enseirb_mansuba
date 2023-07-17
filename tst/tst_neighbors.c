#include "include/tst_neighbors.h"
#include <limits.h>
#include "neighbors.h"

struct test tests_neighbors[] = {
    {tst_init_neighbors, "init_neighbors"},
    {tst_get_neighbors_seed, "get_neighbors_seed"},
    {tst_get_neighbor, "get_neighbor"},
    {tst_get_neighbors, "get_neighbors"},
};

tests tst_neighbors_list() {
    return create_tests(tests_neighbors, TST_COUNT_NEIGHBORS);
}

void tst_init_neighbors() {
    assert(get_neighbors_seed() == MAX_RELATIONS);

    init_neighbors(0);
    assert(get_neighbors_seed() == 0);

    init_neighbors(1);
    assert(get_neighbors_seed() == 1);
}

void tst_get_neighbors_seed() {
    assert(get_neighbors_seed() == MAX_RELATIONS);

    init_neighbors(0);
    assert(get_neighbors_seed() == 0);

    init_neighbors(1);
    assert(get_neighbors_seed() == 1);
}

void tst_get_neighbor() {
    // Seed not initialized: returns nothing
    assert(get_neighbor(0, NORTH) == UINT_MAX);
    assert(get_neighbor(20, NORTH) == UINT_MAX);
    assert(get_neighbor(0, -5) == UINT_MAX);
    assert(get_neighbor(0, 12) == UINT_MAX);

    // seed = 0
    enum dir_t dir[] = {NORTH, SOUTH, EAST, WEST, SWEST, SEAST, NWEST, NEAST};
    unsigned int hope_0_0[] = {UINT_MAX, 5, 1, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};
    unsigned int hope_0_6[] = {1, 11, 7, 5, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};
    unsigned int hope_0_12[] = {7, 17, 13, 11, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};
    unsigned int hope_0_20[] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};

    init_neighbors(0);
    for (int i = 0; i < 8; i++) {
        assert(get_neighbor(0, dir[i]) == hope_0_0[i]);
        assert(get_neighbor(6, dir[i]) == hope_0_6[i]);
        assert(get_neighbor(12, dir[i]) == hope_0_12[i]);
        assert(get_neighbor(20, dir[i]) == hope_0_20[i]);
    }

    // seed = 3
    unsigned int hope_3_0[] = {UINT_MAX, 5, 1, UINT_MAX, UINT_MAX, 6, UINT_MAX, UINT_MAX};
    unsigned int hope_3_6[] = {1, 11, 7, 5, 10, 12, 0, 2};
    unsigned int hope_3_12[] = {7, 17, 13, 11, 16, 18, 6, 8};
    unsigned int hope_3_20[] = {UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX};

    init_neighbors(3);
    for (int i = 0; i < 8; i++) {
        assert(get_neighbor(0, dir[i]) == hope_3_0[i]);
        assert(get_neighbor(6, dir[i]) == hope_3_6[i]);
        assert(get_neighbor(12, dir[i]) == hope_3_12[i]);
        assert(get_neighbor(20, dir[i]) == hope_3_20[i]);
    }
}

// Order defined in geometry.h
// SEAST - SOUTH - SWEST - WEST - EAST - NEAST - NORTH, NWEST
void tst_get_neighbors() {
    // Seed not initialized: returns nothing
    struct neighbors_t r1 = get_neighbors(0);
    struct neighbors_t r2 = get_neighbors(20);
    struct neighbors_t r3 = get_neighbors(-5);
    struct neighbors_t r4 = get_neighbors(12);

    assert(r1.n[0].i == UINT_MAX && r1.n[0].d == NO_DIR);
    assert(r2.n[0].i == UINT_MAX && r2.n[0].d == NO_DIR);
    assert(r3.n[0].i == UINT_MAX && r3.n[0].d == NO_DIR);
    assert(r4.n[0].i == UINT_MAX && r4.n[0].d == NO_DIR);

    // Seed = 0
    init_neighbors(0);

    unsigned int hope_0_0_i[] = {5, 1, UINT_MAX};
    unsigned int hope_0_6_i[] = {11, 5, 7, 1, UINT_MAX};
    unsigned int hope_0_12_i[] = {17, 11, 13, 7, UINT_MAX};
    unsigned int hope_0_20_i[] = {UINT_MAX};
    enum dir_t hope_0_0_d[] = {SOUTH, EAST, NO_DIR};
    enum dir_t hope_0_6_d[] = {SOUTH, WEST, EAST, NORTH, NO_DIR};
    enum dir_t hope_0_12_d[] = {SOUTH, WEST, EAST, NORTH, NO_DIR};
    enum dir_t hope_0_20_d[] = {NO_DIR};

    unsigned int* hope_0_i[] = {hope_0_0_i, hope_0_6_i, hope_0_12_i, hope_0_20_i};
    enum dir_t* hope_0_d[] = {hope_0_0_d, hope_0_6_d, hope_0_12_d, hope_0_20_d};

    struct neighbors_t result_0[] = {get_neighbors(0), get_neighbors(6), get_neighbors(12), get_neighbors(20)};

    int condition;
    for (int i = 0; i < 4; i++) {
        condition = 1;
        for (int j = 0; condition; j++) {
            assert(result_0[i].n[j].i == hope_0_i[i][j]);
            assert(result_0[i].n[j].d == hope_0_d[i][j]);
            if (hope_0_i[i][j] == UINT_MAX)
                condition = 0;
        }
    }

    // Seed = 3
    init_neighbors(3);

    unsigned int hope_3_0_i[] = {6, 5, 1, UINT_MAX};
    unsigned int hope_3_6_i[] = {12, 11, 10, 5, 7, 2, 1, 0, UINT_MAX};
    unsigned int hope_3_12_i[] = {18, 17, 16, 11, 13, 8, 7, 6, UINT_MAX};
    unsigned int hope_3_20_i[] = {UINT_MAX};
    enum dir_t hope_3_0_d[] = {SEAST, SOUTH, EAST, NO_DIR};
    enum dir_t hope_3_6_d[] = {SEAST, SOUTH, SWEST, WEST, EAST, NEAST, NORTH, NWEST, NO_DIR};
    enum dir_t hope_3_12_d[] = {SEAST, SOUTH, SWEST, WEST, EAST, NEAST, NORTH, NWEST, NO_DIR};
    enum dir_t hope_3_20_d[] = {NO_DIR};

    unsigned int* hope_3_i[] = {hope_3_0_i, hope_3_6_i, hope_3_12_i, hope_3_20_i};
    enum dir_t* hope_3_d[] = {hope_3_0_d, hope_3_6_d, hope_3_12_d, hope_3_20_d};

    struct neighbors_t result_3[] = {get_neighbors(0), get_neighbors(6), get_neighbors(12), get_neighbors(20)};

    for (int i = 0; i < 4; i++) {
        condition = 1;
        for (int j = 0; condition; j++) {
            assert(result_3[i].n[j].i == hope_3_i[i][j]);
            assert(result_3[i].n[j].d == hope_3_d[i][j]);
            if (hope_3_i[i][j] == UINT_MAX)
                condition = 0;
        }
    }
}
