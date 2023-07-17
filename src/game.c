#include "game.h"
#include "game_aux.h"
#include "game_config.h"
#include "movement.h"
#include "neighbors.h"

/** Square root of the maximum number of turns for a game, rounded up to the nearest integer */
uint sqrt_max_turn(cgame g) {
    uint i = 0;
    while (i * i < g->gp.max_turns) i++;
    return i;
}

// ************************************************************************* //

/** Find the position in the set according to the player */
uint g_found_idx_of_player(struct ens e, uint idx, uint player) {
    return ens_found_range(e, idx, player * HEIGHT, (player + 1) * HEIGHT);
}

void change_random_seed(game g) {
    // Get a random seed that is not the current neighbors seed
    uint rdm_seed;
    do {
        rdm_seed = get_random_number(MAX_RELATIONS);
    } while (rdm_seed == get_neighbors_seed());

    // Initialize the neighbors with the new seed
    init_neighbors(rdm_seed);

    // Initialize the move scores for the game with the new seed
    game_init_move_scores(g);
}

bool is_same_position(game g, uint idx) {
    bool in_initial_positions = g_found_idx_of_player(g->gs.initial_positions, idx, g->gs.current_player) != UINT_MAX;
    bool in_current_positions = g_found_idx_of_player(g->gs.current_positions, idx, g->gs.current_player) != UINT_MAX;
    bool in_initial_positions_other_player = ens_found_idx(g->gs.initial_positions, idx, 2 * HEIGHT) != UINT_MAX;
    return in_current_positions && !in_initial_positions && in_initial_positions_other_player;
}

void g_update_winning_status(game g) {
    // If the number of turns has reached the maximum, set the winning status to DRAW
    if (g->gs.nb_turns == g->gp.max_turns) {
        g->gs.winning_status = DRAW;
        return;
    }

    uint cpt_same_position = 0;

    // Count the number of positions on the board that have the same color as the current player's piece at that index
    for (uint idx = 0; idx < WORLD_SIZE; idx++)
        cpt_same_position += is_same_position(g, idx);

    // If the game mode is SIMPLE_WIN and there is at least one position with the same color as the current player's piece,
    // set the winning status to SIMPLE_WIN
    if (g->gp.game_mode == SIMPLE_WIN && cpt_same_position >= 1) {
        g->gs.winning_status = SIMPLE_WIN;
        return;
    }

    // If the game mode is COMPLEX_WIN and all positions have the same color as the current player's piece,
    // set the winning status to COMPLEX_WIN
    if (g->gp.game_mode == COMPLEX_WIN && cpt_same_position == HEIGHT) {
        g->gs.winning_status = COMPLEX_WIN;
        return;
    }
}

bool should_change_seed(cgame g) {
    return g->gp.change_random_seed && (g->gs.nb_turns % sqrt_max_turn(g)) == 0 && MAX_RELATIONS > 1;
}

void g_update(game g, struct move_parameters mp) {
    bool seed_changed = false;

    // If the random seed should be changed, change it
    if (should_change_seed(g)) {
        change_random_seed(g);
        seed_changed = true;
    }

    g_update_winning_status(g);
    game_display_logs(g, mp, seed_changed);
}

/* ************************** GAME INITIALIZATION ************************** */

game game_new(enum game_mode gm, int max_turns, int init_player, bool play_with_prisoners, struct ens init_positions) {
    assert(init_player >= -1 && init_player < MAX_PLAYERS);

    game_config game_config = game_configuration_get(0);

    game_configuration_set_game_mode(game_config, gm);
    game_configuration_set_play_with_prisoner(game_config, play_with_prisoners);
    game_configuration_set_init_positions(game_config, init_positions);

    if (max_turns != -1)
        game_configuration_set_max_turns(game_config, max_turns);
    if (init_player != -1)
        game_configuration_set_initial_player(game_config, init_player);

    return game_new_with_config(game_config);
}

game game_new_with_config(game_config game_config) {
    game new_game = malloc(sizeof(struct game));
    assert(new_game);

    new_game->world = world_init();

    game_init_moves(game_config);
    game_init_status(new_game);
    game_init_parameters(new_game, game_config);
    game_init_move_scores(new_game);

    g_update_winning_status(new_game);
    game_configuration_delete(game_config);

    return new_game;
}

/* ******************************** GETTERS ******************************** */

enum color_t game_get_color(cgame g, uint idx) {
    assert(g);
    assert(is_valid_idx(idx));
    return world_get(g->world, idx);
}

enum sort_t game_get_sort(cgame g, uint idx) {
    assert(g);
    assert(is_valid_idx(idx));
    return world_get_sort(g->world, idx);
}

enum game_mode game_get_winning_status(cgame g) {
    assert(g);
    return g->gs.winning_status;
}

uint game_get_current_player(cgame g) {
    assert(g);
    return g->gs.current_player;
}

bool game_play_with_prisoner(cgame g) {
    assert(g);
    return g->gp.play_with_prisoner;
}

/* ***************************** GAME MOVEMENTS **************************** */

// Gets the index of an unbound piece in game g for the specified player
// using the specified piece index and the current position of the piece in the current positions set.
// If no piece is found, returns UINT_MAX. This part should never happen.
uint get_not_prisoner_piece_index(cgame g, uint idx, uint player) {
    // Iterate through the player's current positions set
    for (uint i = (player * HEIGHT); i < (player + 1) * HEIGHT; i++) {
        // If the current position contains the index of the sought piece
        if (ens_get_val(g->gs.current_positions, i) == idx) {
            // If the piece is not bound, return its index
            if (!ens_get_val(g->gs.current_prisoners, i))
                return i;
        }
    }
    return UINT_MAX; // Should never happen
}

void g_capture_sort(game g, uint idx) {
    uint other_player = (game_get_current_player(g) + 1) % 2;
    uint id_pos = get_not_prisoner_piece_index(g, idx, other_player);
    assert(id_pos != UINT_MAX); // should never happen

    // Makes the game piece prisoner and saves its data
    ens_set_val(&g->gs.current_prisoners, id_pos, 1);
    ens_set_sort(&g->gs.current_prisoners, id_pos, game_get_sort(g, idx));
    ens_set_color(&g->gs.current_prisoners, id_pos, game_get_color(g, idx));
}

void game_move_random(game g) {
    assert(g);
    uint id_src = ens_get_val(g->gs.current_positions, get_random_number(HEIGHT) + g->gs.current_player * HEIGHT);
    uint id_dst = UINT_MAX;

    uint id_ens = g_found_idx_of_player(g->gs.current_positions, id_src, g->gs.current_player);

    if (!ens_get_val(g->gs.current_prisoners, id_ens))
        id_dst = get_random_movement(g, id_src);
    game_play_move(g, id_src, id_dst);
}

void make_move(game g, uint id_src, uint id_dst) {
    assert(is_valid_idx(id_src));
    assert(is_valid_idx(id_dst));

    // Takes the data from the id_src position and copies it to the id_dst position
    g_set_color(g, id_dst, game_get_color(g, id_src));
    g_set_sort(g, id_dst, game_get_sort(g, id_src));

    // Removes the game piece at position id_src from the game
    g_set_color(g, id_src, NO_COLOR);
    g_set_sort(g, id_src, NO_SORT);

    uint id_ens = g_found_idx_of_player(g->gs.current_positions, id_src, g->gs.current_player);

    ens_set_val(&g->gs.current_positions, id_ens, id_dst);
}

bool is_valid_move(uint id_dst, struct ens apm) {
    return id_dst != UINT_MAX && ens_get_val(apm, id_dst) == MOVE_POSSIBLE;
}

bool is_escape_move(cgame g, uint id_src) {
    uint id_ens = g_found_idx_of_player(g->gs.current_positions, id_src, g->gs.current_player);
    return ens_get_val(g->gs.current_prisoners, id_ens);
}

bool is_valid_dst(uint id_dst) {
    return id_dst != UINT_MAX;
}

bool is_capture_move(cgame g, uint id_dst) {
    return game_get_sort(g, id_dst) != NO_SORT && g->gp.play_with_prisoner;
}

enum move_t try_escape(game g, uint id_src) {
    // Escape not possible if a piece is already at id_src position
    if (game_get_color(g, id_src) == NO_COLOR) {
        // 50% chance (1/2)
        if (get_random_number(2)) {
            uint id_ens = g_found_idx_of_player(g->gs.current_positions, id_src, g->gs.current_player);

            // The game piece reappears in play
            g_set_color(g, id_src, ens_get_color(g->gs.current_prisoners, id_ens));
            g_set_sort(g, id_src, ens_get_sort(g->gs.current_prisoners, id_ens));

            // The game piece is no longer a prisoner
            ens_set_val(&g->gs.current_prisoners, id_ens, false);

            return ESCAPE;
        }
    }
    return ESCAPE_FAILED;
}

enum move_t try_capture(game g, uint id_dst) {
    if (is_capture_move(g, id_dst)) {
        g_capture_sort(g, id_dst);
        return MOVE_WITH_CAPTURE;
    }
    return MOVE_POSSIBLE;
}

enum move_t try_move(game g, struct ens* apm, uint id_src, uint id_dst) {
    enum move_t move = MOVE_NOT_POSSIBLE;

    // Gets all possible moves from the id_src position
    get_all_possible_moves(g, id_src, apm);

    // If the move from apm on id_dst position is valid
    if (is_valid_move(id_dst, *apm)) {
        // A capture is tried
        move = try_capture(g, id_dst);
        make_move(g, id_src, id_dst);
    }
    return move;
}

void game_play_move(game g, uint id_src, uint id_dst) {
    assert(g);
    assert(is_valid_idx(id_src));
    assert(is_valid_idx(id_dst || id_dst == UINT_MAX));

    struct ens apm = ens_create(NOT_VISITED);
    struct move_parameters mp = {MOVE_NOT_POSSIBLE, id_src, id_dst, &apm, g->gs.current_player};

    if (is_escape_move(g, id_src))
        mp.move = try_escape(g, id_src);
    else if (is_valid_dst(id_dst))
        mp.move = try_move(g, &apm, id_src, id_dst);

    g->gs.nb_turns++;
    g_update(g, mp);
}

bool game_is_over(cgame g) {
    assert(g);
    return game_get_winning_status(g) != NO_WIN;
}

bool game_is_prisoner(cgame g, uint idx, uint player) {
    assert(g);
    assert(is_valid_idx(idx));
    assert(player < MAX_PLAYERS);

    uint id_ens = g_found_idx_of_player(g->gs.current_positions, idx, player);
    return ens_get_val(g->gs.current_prisoners, id_ens);
}

void game_next_player(game g) {
    assert(g);
    g->gs.current_player = (g->gs.current_player + 1) % 2;
}

void game_delete(game g) {
    free(g);
}

game_config game_get_configuration(int id_config) {
    return game_configuration_get(id_config);
}
