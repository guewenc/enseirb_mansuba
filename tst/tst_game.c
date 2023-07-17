#include "include/tst_game.h"
#include "game.h"
#include "movement.h"

struct test tests_game[] = {
    {tst_game_new, "game_new"},
    {tst_game_next_player, "game_next_player"},
    {tst_game_get_current_player, "game_get_current_player"},
    {tst_game_play_move, "game_play_move"},
    {tst_game_is_over, "game_is_over"},
    {tst_game_play_with_prisoner, "game_play_with_prisoner"},
    {tst_game_get_color, "game_get_color"},
    {tst_game_get_sort, "game_get_sort"},
    {tst_game_get_win_status, "game_get_win_status"},
    {tst_game_delete, "game_delete"}};

tests tst_game_list() {
    return create_tests(tests_game, TST_COUNT_GAME);
}

void tst_game_new() {
    game g = game_new(SIMPLE_WIN, -1, -1, false, ens_create(UINT_MAX));

    assert(!game_is_over(g));
    assert(!game_play_with_prisoner(g));

    game_delete(g);
    g = game_new(SIMPLE_WIN, 0, 0, true, ens_create(UINT_MAX));
    assert(game_get_current_player(g) == 0);
    assert(game_is_over(g));
    assert(game_play_with_prisoner(g));

    game_delete(g);

    struct ens init_pos = ens_create(UINT_MAX);
    ens_set_val(&init_pos, 0, 0);
    ens_set_val(&init_pos, 1, 5);
    ens_set_val(&init_pos, 2, 10);
    ens_set_val(&init_pos, 3, 15);
    g = game_new(SIMPLE_WIN, 0, 0, true, init_pos);
    for (uint i = 0; i < WORLD_SIZE; i++) {
        if (i % 5 == 0 || i % 5 == 4) assert(game_get_color(g, i) != NO_COLOR);
        else
            assert(game_get_color(g, i) == NO_COLOR);
    }
    game_delete(g);
}

void tst_game_next_player() {
    game g = game_new(SIMPLE_WIN, -1, 0, false, ens_create(UINT_MAX));
    game_next_player(g);
    assert(game_get_current_player(g) == 1);
    game_next_player(g);
    assert(game_get_current_player(g) == 0);
    game_delete(g);
}

void tst_game_get_current_player() {
    game g = game_new(SIMPLE_WIN, -1, 0, false, ens_create(UINT_MAX));
    assert(game_get_current_player(g) == 0);
    game_next_player(g);
    assert(game_get_current_player(g) == 1);
    game_delete(g);
    g = game_new(SIMPLE_WIN, -1, 1, false, ens_create(UINT_MAX));
    assert(game_get_current_player(g) == 1);
    game_delete(g);
}

void tst_game_play_move() {
    struct ens init_pos = ens_create(UINT_MAX);
    ens_fill(&init_pos, 0, 0, NO_COLOR, PAWN);
    ens_fill(&init_pos, 1, 5, NO_COLOR, PAWN);
    ens_fill(&init_pos, 2, 10, NO_COLOR, PAWN);
    ens_fill(&init_pos, 3, 15, NO_COLOR, PAWN);
    game g = game_new(SIMPLE_WIN, -1, 0, true, init_pos);
    init_neighbors(0);

    game_play_move(g, 0, 1);
    assert(game_get_color(g, 0) == NO_COLOR);
    assert(game_get_color(g, 1) == WHITE);
    assert(game_get_sort(g, 0) == NO_SORT);
    assert(game_get_sort(g, 1) != NO_SORT);

    game_play_move(g, 1, 2);
    assert(game_get_color(g, 1) == NO_COLOR);
    assert(game_get_color(g, 2) == WHITE);
    assert(game_get_sort(g, 1) == NO_SORT);
    assert(game_get_sort(g, 2) != NO_SORT);

    game_next_player(g);

    game_play_move(g, 4, 3);
    assert(game_get_color(g, 4) == NO_COLOR);
    assert(game_get_color(g, 3) == BLACK);
    assert(game_get_sort(g, 4) == NO_SORT);
    assert(game_get_sort(g, 3) != NO_SORT);

    game_delete(g);
}

void tst_game_is_over() {
    game g = game_new(SIMPLE_WIN, 1, 0, false, ens_create(UINT_MAX));
    assert(!game_is_over(g));
    game_delete(g);

    struct ens init_pos = ens_create(UINT_MAX);
    ens_fill(&init_pos, 0, 0, NO_COLOR, TOWER);
    ens_fill(&init_pos, 1, 11, NO_COLOR, PAWN);
    ens_fill(&init_pos, 2, 10, NO_COLOR, PAWN);
    ens_fill(&init_pos, 3, 15, NO_COLOR, PAWN);
    g = game_new(SIMPLE_WIN, -1, 0, true, init_pos);
    init_neighbors(0);

    game_play_move(g, 0, 5);
    assert(!game_is_over(g));
    game_next_player(g);
    game_play_move(g, 4, 0);
    assert(game_is_over(g));
    game_delete(g);
}

void tst_game_play_with_prisoner() {
    game g = game_new(SIMPLE_WIN, -1, 0, false, ens_create(UINT_MAX));
    assert(!game_play_with_prisoner(g));
    game_delete(g);
    g = game_new(SIMPLE_WIN, -1, 0, true, ens_create(UINT_MAX));
    assert(game_play_with_prisoner(g));
    game_delete(g);
}

void tst_game_get_color() {
    struct ens init_pos = ens_create(UINT_MAX);
    ens_set_val(&init_pos, 0, 0);
    ens_set_val(&init_pos, 1, 5);
    ens_set_val(&init_pos, 2, 10);
    ens_set_val(&init_pos, 3, 15);
    game g = game_new(SIMPLE_WIN, 0, 0, true, init_pos);
    for (uint i = 0; i < WORLD_SIZE; i++) {
        if (i % 5 == 0) assert(game_get_color(g, i) == WHITE);
        else if (i % 5 == 4)
            assert(game_get_color(g, i) == BLACK);
        else
            assert(game_get_color(g, i) == NO_COLOR);
    }
    game_delete(g);
}

void tst_game_get_sort() {
    struct ens init_pos = ens_create(UINT_MAX);
    ens_set_val(&init_pos, 0, 0);
    ens_set_val(&init_pos, 1, 5);
    ens_set_val(&init_pos, 2, 10);
    ens_set_val(&init_pos, 3, 15);
    game g = game_new(SIMPLE_WIN, 0, 0, true, init_pos);
    for (uint i = 0; i < WORLD_SIZE; i++) {
        if (i % 5 == 0 || i % 5 == 4) assert(game_get_sort(g, i) != NO_SORT);
        else
            assert(game_get_sort(g, i) == NO_SORT);
    }
    game_delete(g);
}

void tst_game_get_win_status() {
    game g = game_new(SIMPLE_WIN, 1, 0, false, ens_create(UINT_MAX));
    assert(game_get_winning_status(g) == NO_WIN);
    game_delete(g);

    struct ens init_pos = ens_create(UINT_MAX);
    ens_fill(&init_pos, 0, 0, NO_COLOR, TOWER);
    ens_fill(&init_pos, 1, 11, NO_COLOR, PAWN);
    ens_fill(&init_pos, 2, 10, NO_COLOR, PAWN);
    ens_fill(&init_pos, 3, 15, NO_COLOR, PAWN);
    g = game_new(SIMPLE_WIN, -1, 0, true, init_pos);
    init_neighbors(0);

    game_play_move(g, 0, 5);
    game_next_player(g);
    game_play_move(g, 4, 0);
    assert(game_get_winning_status(g) == SIMPLE_WIN);
    game_delete(g);
}

void tst_game_delete() {
    game g = game_new(SIMPLE_WIN, -1, -1, false, ens_create(UINT_MAX));
    game_delete(g);
}