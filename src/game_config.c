#include "game_config.h"

/* Basic game :
    - Game mode : Simple win
    - Grid : Basic grid (seed 0)
    - Play with prisoners : Yes
    - Seed can change : Yes
    - All sort and basics moves
*/
game_config game_configuration_get_basic() {
    game_config new_c = game_configuration_new_empty();

    game_configuration_set_game_mode(new_c, SIMPLE_WIN);
    game_configuration_set_grid_mode(new_c, GRID);
    game_configuration_set_play_with_prisoner(new_c, true);
    game_configuration_set_change_seed(new_c, true);

    game_configuration_enable_all_sort(new_c);

    movement moves[MAX_SORT - 1][MAX_MOVE_TYPES] = {
        {&simple_move, &multiple_jump},
        {&move_toward_same_dir},
        {&double_jump}};
    game_configuration_set_all_movement(new_c, moves);

    return new_c;
}

/* Chinese checkers game :
    - Game mode : Simple win
    - Grid : Triangle
    - Play with prisoners : No
    - Seed can change : No
    - Only pawn
*/
game_config game_configuration_get_chinese_checkers() {
    game_config new_c = game_configuration_new_empty();

    game_configuration_set_game_mode(new_c, SIMPLE_WIN);
    game_configuration_set_grid_mode(new_c, TRIANGLE);
    game_configuration_set_play_with_prisoner(new_c, false);
    game_configuration_set_change_seed(new_c, false);

    game_configuration_enable_sort(new_c, PAWN);

    movement move_pawn[MAX_MOVE_TYPES] = {&simple_move, &multiple_jump};
    game_configuration_set_movement(new_c, PAWN, move_pawn);

    return new_c;
}

game_config game_configuration_new_empty() {
    game_config new_gc = malloc(sizeof(struct game_config));
    assert(new_gc);

    game_configuration_set_max_turns(new_gc, 2 * WORLD_SIZE);
    game_configuration_set_initial_player(new_gc, get_random_number(2));

    game_configuration_set_init_positions(new_gc, ens_create(UINT_MAX));

    return new_gc;
}

void game_configuration_delete(game_config gc) {
    free(gc);
}

game_config game_configuration_get(int id_config) {
    switch (id_config) {
        case 0:
            return game_configuration_get_basic();
            break;
        case 1:
            return game_configuration_get_chinese_checkers();
            break;
        default: break;
    }
    return game_configuration_get_basic();
}

/* ************************************************************************ */
/*                                  SETTERS                                 */
/* ************************************************************************ */

void game_configuration_set_max_turns(game_config gc, uint max_turns) {
    gc->game_param.max_turns = max_turns;
}

void game_configuration_set_play_with_prisoner(game_config gc, bool play_with_prisoner) {
    gc->game_param.play_with_prisoner = play_with_prisoner;
}

void game_configuration_set_game_mode(game_config gc, enum game_mode game_mode) {
    gc->game_param.game_mode = game_mode;
}

void game_configuration_set_grid_mode(game_config gc, enum grid_mode grid_mode) {
    gc->game_param.grid_mode = grid_mode;
}

void game_configuration_set_change_seed(game_config gc, bool change_seed) {
    gc->game_param.change_random_seed = change_seed;
}

void game_configuration_set_initial_player(game_config gc, uint initial_player) {
    assert(initial_player < MAX_PLAYERS);
    gc->game_param.initial_player = initial_player;
}

void game_configuration_set_init_positions(game_config gc, struct ens init_positions) {
    gc->initial_positions = init_positions;
}

void game_configuration_enable_sort(game_config gc, enum sort_t sort) {
    assert(sort > NO_SORT && sort < MAX_SORT);
    gc->piece_config[sort - 1].play_with = true;
}

void game_configuration_enable_all_sort(game_config gc) {
    for (int sort = 1; sort < MAX_SORT; sort++)
        game_configuration_enable_sort(gc, sort);
}

void game_configuration_disable_sort(game_config gc, enum sort_t sort) {
    assert(sort > NO_SORT && sort < MAX_SORT);
    gc->piece_config[sort - 1].play_with = false;
}

void game_configuration_disable_all_sort(game_config gc) {
    for (int sort = 1; sort < MAX_SORT; sort++)
        game_configuration_enable_sort(gc, sort);
}

void game_configuration_set_movement(game_config gc, enum sort_t sort, movement m[MAX_MOVE_TYPES]) {
    assert(sort > NO_SORT && sort < MAX_SORT);

    // Initialise the array to NULL to avoid any memory access problems due to uninitialised cells.
    for (int i = 0; i < MAX_MOVE_TYPES; i++)
        gc->piece_config[sort - 1].m[i] = NULL;
    for (int i = 0; i < MAX_MOVE_TYPES && m[i]; i++)
        gc->piece_config[sort - 1].m[i] = m[i];
}

void game_configuration_set_all_movement(game_config gc, movement m[MAX_SORT - 1][MAX_MOVE_TYPES]) {
    for (int sort = 1; sort < MAX_SORT; sort++)
        game_configuration_set_movement(gc, sort, m[sort - 1]);
}
