#include "utils.h"
#include "geometry.h"

int get_random_number(int range) {
    return rand() % range;
}

bool is_valid_idx(uint idx) {
    return idx < WORLD_SIZE;
}
