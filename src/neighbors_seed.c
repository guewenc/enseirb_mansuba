#include "neighbors_seed.h"
#include "position.h"

enum dir_t dir_order[] = {NORTH, EAST, SOUTH, WEST, SWEST, NWEST, SEAST, NEAST};

uint get_idx_dir(enum dir_t d) {
    for (int i = 0; i < 8; i++)
        if (dir_order[i] == d)
            return i;
    return UINT_MAX;
}

struct pos seeds[][8] = {
    { // Seed 0
        {0, -1}, {1, 0}, {0, 1}, {-1, 0},
        {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}
    },
    { // Seed 1 --> Case even
        {-INT_MAX, INT_MAX}, {1, 0}, {-INT_MAX, INT_MAX}, {-1, 0},
        {-1, 1}, {-1, -1}, {1, 1}, {1, -1}
    },
    { // Seed 1 --> Case not even
        {-INT_MAX, INT_MAX}, {1, 0}, {-INT_MAX, INT_MAX}, {-1, 0},
        {0, 1}, {0, -1}, {1, 1}, {1, -1}
    },
    { // Seed 2 --> Case even
        {1, -1}, {1, 0}, {-INT_MAX, INT_MAX}, {-1, 0},
        {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}
    },
    { // Seed 2 --> Case not even
        {-1, 1}, {1, 0}, {-INT_MAX, INT_MAX}, {-1, 0},
        {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}, {-INT_MAX, INT_MAX}
    },
    { // Seed 3
        {0, -1}, {1, 0}, {0, 1}, {-1, 0},
        {-1, 1}, {-1, -1}, {1, 1}, {1, -1}
    }
};

bool is_forbidden_translation(struct pos p) {
    return get_x(p) == -INT_MAX && get_y(p) == INT_MAX;
}

uint get_neighbor_of_seed(uint seed, uint idx, enum dir_t d) {
    struct pos p_idx = idx_to_pos(idx);

    int idx_seed;

    switch (seed) {
        case 0:
            idx_seed = 0;
            break;
        case 1:
            idx_seed = 1 + get_y(p_idx) % 2;
            break;
        case 2:
            if (WIDTH % 2 == 1 && get_y(p_idx) % 2 == 1) idx++;
            idx_seed = 3 + idx % 2;
            break;
        case 3:
            idx_seed = 5;
            break;
        default:
            return UINT_MAX;
    }

    struct pos translation = seeds[idx_seed][get_idx_dir(d)];

    if (is_forbidden_translation(translation))
        return UINT_MAX;
    return pos_to_idx(pos_translate(p_idx, translation));
}
