#include "neighbors.h"

#include "neighbors_seed.h"
#include "position.h"
#include "utils.h"

uint selected_seed = MAX_RELATIONS; // Make sure that seed is not initialized

unsigned int get_neighbors_seed() {
    return selected_seed;
}

void init_neighbors(uint seed) {
    assert(seed < MAX_RELATIONS);
    selected_seed = seed;
}

uint get_neighbor(uint idx, enum dir_t d) {
    if (is_inside(idx))
        return get_neighbor_of_seed(get_neighbors_seed(), idx, d);
    return UINT_MAX;
}

struct vector_t make_vector(uint i, enum dir_t d) {
    return (struct vector_t){i, d};
}

struct neighbors_t get_neighbors(uint idx) {
    struct neighbors_t neighbors;
    int index = 0;

    // For all defined directions
    for (enum dir_t d = -4; d <= 4; d == -1 ? d += 2 : d++) {
        uint n = get_neighbor(idx, d);
        if (n != UINT_MAX)
            neighbors.n[index++] = make_vector(n, d);
    }
    neighbors.n[index] = make_vector(UINT_MAX, NO_DIR);
    return neighbors;
}
