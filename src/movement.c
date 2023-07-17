#include "movement.h"
#include "game_aux.h"

enum color_t act_player_color; /* Color associated to the actual player*/
bool has_moved; /* Boolean value indicating if pawn has already move or not while calculating multiples moves */
uint last_move; /* Index of the destination of the last move */
movement moves_per_sort[MAX_SORT - 1][MAX_MOVE_TYPES]; /* List of the authorized moves per piece */

void define_moves_per_sort(enum sort_t sort, movement m[MAX_MOVE_TYPES]) {
    for (int i = 0; i < MAX_MOVE_TYPES; i++)
        moves_per_sort[sort - 1][i] = NULL;
    for (int i = 0; i < MAX_MOVE_TYPES && m[i]; i++)
        moves_per_sort[sort - 1][i] = m[i];
}

bool is_initial_position_of_other(cgame g, uint idx) {
    bool found_init_pos = ens_found_idx(g->gs.initial_positions, idx, 2 * HEIGHT) != UINT_MAX;
    bool found_init_pos_of_other = ens_found_range(g->gs.initial_positions, idx, g->gs.current_player * HEIGHT, (g->gs.current_player + 1) * HEIGHT) != UINT_MAX;
    return found_init_pos && !found_init_pos_of_other;
}

bool can_capture(cgame g, uint idx) {
    return game_play_with_prisoner(g) && !is_initial_position_of_other(g, idx);
}

bool check_move(cgame g, uint idx) {
    return game_get_sort(g, idx) == NO_SORT || (can_capture(g, idx) && game_get_color(g, idx) != act_player_color);
}

void set_move_possible(struct ens* apm, uint idx) {
    if (ens_get_val(*apm, idx) == MOVE_POSSIBLE) return;
    has_moved = true;
    last_move = idx;
    ens_set_val(apm, idx, MOVE_POSSIBLE);
}

void set_move_not_possible(struct ens* apm, uint idx) {
    ens_set_val(apm, idx, MOVE_NOT_POSSIBLE);
}

void simple_move_toward(struct ens* apm, cgame g, uint idx, enum dir_t d, bool rec) {
    uint neigh = get_neighbor(idx, d);
    if (neigh != UINT_MAX)
        if (check_move(g, neigh)) {
            set_move_possible(apm, neigh);

            // Recursive call is made only if the last move didn't end on a piece.
            if (rec && game_get_sort(g, neigh) == NO_SORT)
                simple_move_toward(apm, g, neigh, d, rec);
        }
}

void simple_move(struct ens* apm, cgame g, uint idx) {
    struct neighbors_t n = get_neighbors(idx);
    for (int i = 0; IDX(n) != UINT_MAX; i++) {
        if (check_move(g, IDX(n)))
            set_move_possible(apm, IDX(n));
        else
            set_move_not_possible(apm, IDX(n));
    }
}

void simple_jump_rec(struct ens* apm, cgame g, uint idx, bool rec) {
    struct neighbors_t n = get_neighbors(idx);
    for (int i = 0; IDX(n) != UINT_MAX; i++) {
        // Verifies if there is a piece to jump over.
        if (game_get_color(g, IDX(n)) != NO_COLOR) {
            // Verifies if there is an empty place to move to behind the jumped piece.
            has_moved = false;
            simple_move_toward(apm, g, IDX(n), DIR(n), false);

            // If previous jump was possible, has_moved is True and it's possible to make a recursive call (if rec is True)
            if (rec && has_moved)
                simple_jump_rec(apm, g, last_move, true);
        }
    }
}

void simple_jump(struct ens* apm, cgame g, uint idx) {
    simple_jump_rec(apm, g, idx, false);
}

void double_jump(struct ens* apm, cgame g, uint idx) {
    struct neighbors_t n = get_neighbors(idx);
    for (int i = 0; IDX(n) != UINT_MAX; i++)
        simple_move(apm, g, IDX(n));
}

void multiple_jump(struct ens* apm, cgame g, uint idx) {
    simple_jump_rec(apm, g, idx, true);
}

void move_toward_same_dir(struct ens* apm, cgame g, uint idx) {
    struct neighbors_t n = get_neighbors(idx);
    for (int i = 0; IDX(n) != UINT_MAX; i++) {
        if (check_move(g, IDX(n))) {
            set_move_possible(apm, IDX(n));

            // Recursive call is made only if the last move didn't end on a piece.
            if (game_get_sort(g, IDX(n)) == NO_SORT)
                simple_move_toward(apm, g, IDX(n), DIR(n), true);
        } else
            set_move_not_possible(apm, IDX(n));
    }
}

void get_all_possible_moves(cgame g, uint idx, struct ens* apm) {
    act_player_color = game_get_color(g, idx);
    enum sort_t sort = game_get_sort(g, idx) - 1;

    for (int i = 0; i < MAX_MOVE_TYPES && moves_per_sort[sort][i]; i++)
        moves_per_sort[sort][i](apm, g, idx);

    ens_set_val(apm, idx, MOVE_NOT_POSSIBLE);
}

uint get_other_player(uint p) { return p ? 0 : 1; }

uint select_random_id(cgame g, struct ens apm) {
    // Initializes values to find the best moves possible.
    uint available_values[WORLD_SIZE];
    uint available_values_count = 0;
    uint best_score = WORLD_SIZE;

    for (uint i = 0; i < WORLD_SIZE; i++) {
        enum move_t move = ens_get_val(apm, i);
        if (move == MOVE_POSSIBLE) {
            // If a move is possible, we get its score to compare it to previous moves.
            uint score = ens_get_val(g->score_init_pos[get_other_player(game_get_current_player(g))], i);

            if (score == best_score)
                available_values[available_values_count++] = i;
            else if (score < best_score) {
                best_score = score;
                available_values_count = 0;
                available_values[available_values_count++] = i;
            }
        }
    }

    return available_values_count ? available_values[get_random_number(available_values_count)] : UINT_MAX;
}

uint get_random_movement(cgame g, uint id_src) {
    struct ens apm = ens_create(NOT_VISITED);
    get_all_possible_moves(g, id_src, &apm);
    return select_random_id(g, apm);
}
