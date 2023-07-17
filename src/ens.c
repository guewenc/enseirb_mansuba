#include "ens.h"

void ens_fill(struct ens* e, uint idx, uint val, enum color_t c, enum sort_t s) {
    ens_set_val(e, idx, val);
    ens_set_color(e, idx, c);
    ens_set_sort(e, idx, s);
}

struct ens ens_create(uint val) {
    struct ens ne;
    for (uint i = 0; i < WORLD_SIZE; i++)
        ens_fill(&ne, i, val, NO_COLOR, NO_SORT);
    return ne;
}

uint ens_found_range(struct ens e, uint idx, uint r_inf, uint r_sup) {
    for (uint i = r_inf; i < r_sup; i++)
        if (ens_get_val(e, i) == idx)
            return i;
    return UINT_MAX;
}

uint ens_found_idx(struct ens e, uint idx, int size) {
    return ens_found_range(e, idx, 0, size);
}

void ens_set_val(struct ens* e, uint idx, uint val) {
    assert(e);
    assert(is_valid_idx(idx));
    e->place[idx].val = val;
}

void ens_set_sort(struct ens* e, uint idx, enum sort_t sort) {
    assert(e);
    assert(is_valid_idx(idx));
    e->place[idx].sort = sort;
}

void ens_set_color(struct ens* e, uint idx, enum color_t color) {
    assert(e);
    assert(is_valid_idx(idx));
    e->place[idx].color = color;
}

uint ens_get_val(struct ens e, uint idx) {
    assert(is_valid_idx(idx));
    return e.place[idx].val;
}

enum sort_t ens_get_sort(struct ens e, uint idx) {
    assert(is_valid_idx(idx));
    return e.place[idx].sort;
}

enum color_t ens_get_color(struct ens e, uint idx) {
    assert(is_valid_idx(idx));
    return e.place[idx].color;
}
