#include "include/tst_position.h"
#include "position.h"

struct test tests_position[] = {
    {tst_get_x, "get_x"},
    {tst_get_y, "get_y"},
    {tst_make_pos, "make_pos"},
    {tst_idx_to_pos, "idx_to_pos"},
    {tst_pos_to_idx, "pos_to_idx"},
    {tst_is_inside, "is_inside"},
    {tst_is_inside_pos, "is_inside_pos"},
    {tst_pos_translate, "pos_translate"},
    {tst_pos_are_equals, "pos_are_equals"},
    {tst_display_pos, "display_pos"},
};

tests tst_position_list() {
    return create_tests(tests_position, TST_COUNT_POSITION);
}

void tst_get_x() {
    struct pos p[] = {make_pos(1, 2), make_pos(-1, 1), make_pos(0, 3)};
    int hope[] = {1, -1, 0};

    for (int i = 0; i < 3; i++)
        assert(get_x(p[i]) == hope[i]);
}

void tst_get_y() {
    struct pos p[] = {make_pos(1, 2), make_pos(-1, 1), make_pos(0, 3)};
    int hope[] = {2, 1, 3};

    for (int i = 0; i < 3; i++)
        assert(get_y(p[i]) == hope[i]);
}

void tst_make_pos() {
    struct pos p[] = {make_pos(1, 2), make_pos(-1, 1), make_pos(0, 3)};

    int hope_x[] = {1, -1, 0};
    int hope_y[] = {2, 1, 3};

    for (int i = 0; i < 3; i++)
        assert(p[i].x == hope_x[i] && p[i].y == hope_y[i]);
}

void tst_idx_to_pos() {
    struct pos hope[] = {make_pos(1, 0), make_pos(0, 1), make_pos(3, 2), make_pos(4, 3)};
    struct pos p[] = {idx_to_pos(1), idx_to_pos(5), idx_to_pos(13), idx_to_pos(19)};

    for (int i = 0; i < 4; i++)
        assert(pos_are_equals(p[i], hope[i]));
}

void tst_pos_to_idx() {
    struct pos p[] = {make_pos(1, 0), make_pos(0, 1), make_pos(3, 2), make_pos(4, 3)};
    int hope[] = {1, 5, 13, 19};

    for (int i = 0; i < 4; i++)
        assert(pos_to_idx(p[i]) == hope[i]);
}

void tst_is_inside() {
    for (int i = 0; i < 20; i++)
        assert(is_inside(i));
    assert(!is_inside(-1));
    assert(!is_inside(20));
    assert(!is_inside(UINT_MAX));
    assert(!is_inside(-5));
}

void tst_is_inside_pos() {
    struct pos p_test_inside[] = {make_pos(0, 0), make_pos(0, 3), make_pos(4, 3), make_pos(2, 2), make_pos(3, 0)};
    struct pos p_test_not_inside[] = {make_pos(-1, 0), make_pos(0, -1), make_pos(5, 3), make_pos(5, 4), make_pos(UINT_MAX, 0)};

    for (int i = 0; i < 5; i++) {
        assert(is_inside_pos(p_test_inside[i]));
        assert(!is_inside_pos(p_test_not_inside[i]));
    }
}

void tst_pos_translate() {
    struct pos p[] = {make_pos(0, 0), make_pos(-1, 1), make_pos(-5, 12), make_pos(4, 9), make_pos(7, 7)};
    struct pos t[] = {make_pos(0, 0), make_pos(-1, -1), make_pos(3, 3), make_pos(1, 4), make_pos(4, 0)};
    struct pos hope[] = {make_pos(0, 0), make_pos(-2, 0), make_pos(-2, 15), make_pos(5, 13), make_pos(11, 7)};

    for (int i = 0; i < 5; i++)
        assert(pos_are_equals(pos_translate(p[i], t[i]), hope[i]));
}

void tst_pos_are_equals() {
    struct pos p_equals[] = {make_pos(0, 0), make_pos(-1, 1), make_pos(-5, 12), make_pos(4, 9), make_pos(7, 7)};
    struct pos p_not_equals[] = {make_pos(-1, 0), make_pos(1, 1), make_pos(-5, 123), make_pos(0, 9), make_pos(6, 7)};
    struct pos hope[] = {{0, 0}, {-1, 1}, {-5, 12}, {4, 9}, {7, 7}};

    for (int i = 0; i < 5; i++) {
        assert(pos_are_equals(p_equals[i], hope[i]));
        assert(!pos_are_equals(p_not_equals[i], hope[i]));
    }
}

void tst_display_pos() {
    struct pos p[] = {make_pos(1, 2), make_pos(-1, 2), make_pos(0, 0)};

    for (int i = 0; i < 3; i++)
        display_pos(p[i]);
}
