#include "position.h"

int get_x(struct pos p) { return p.x; }
int get_y(struct pos p) { return p.y; }

struct pos make_pos(int x, int y) {
    return (struct pos){x, y};
}

struct pos idx_to_pos(int idx) {
    return make_pos(idx % WIDTH, idx / WIDTH);
}

int pos_to_idx(struct pos p) {
    if (is_inside_pos(p))
        return get_y(p) * WIDTH + get_x(p);
    return UINT_MAX;
}

struct pos pos_translate(struct pos p, struct pos t) {
    return make_pos(get_x(p) + get_x(t), get_y(p) + get_y(t));
}

bool is_inside(int idx) {
    return idx >= 0 && idx < WORLD_SIZE;
}

bool is_inside_pos(struct pos p) {
    return get_x(p) >= 0 && get_y(p) >= 0 && get_x(p) < WIDTH && get_y(p) < HEIGHT;
}

bool pos_are_equals(struct pos p1, struct pos p2) {
    return get_x(p1) == get_x(p2) && get_y(p1) == get_y(p2);
}

void display_pos(struct pos p) {
    fprintf(stdout, "Position : (%d, %d)\n", get_x(p), get_y(p));
}
