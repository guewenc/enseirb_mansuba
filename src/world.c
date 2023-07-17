#include "world.h"
#include "utils.h"

struct world_t {
    struct place_t {
        enum sort_t sort;
        enum color_t color;
    } place[WORLD_SIZE];
};

struct world_t world;

struct world_t* world_init() {
    for (int i = 0; i < WORLD_SIZE; i++) {
        world_set(&world, i, NO_COLOR);
        world_set_sort(&world, i, NO_SORT);
    }
    return &world;
}

enum color_t world_get(const struct world_t* b, unsigned int idx) {
    assert(is_valid_idx(idx));
    return b->place[idx].color;
}

enum sort_t world_get_sort(const struct world_t* b, unsigned int idx) {
    assert(is_valid_idx(idx));
    return b->place[idx].sort;
}

void world_set(struct world_t* b, unsigned int idx, enum color_t c) {
    assert(is_valid_idx(idx));
    b->place[idx].color = c;
}

void world_set_sort(struct world_t* b, unsigned int idx, enum sort_t s) {
    assert(is_valid_idx(idx));
    b->place[idx].sort = s;
}
