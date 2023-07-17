#include "game_aux.h"
#include "colors.h"

void g_set_color(game g, uint idx, enum color_t c) {
    world_set(g->world, idx, c);
}

void g_set_sort(game g, uint idx, enum sort_t s) {
    world_set_sort(g->world, idx, s);
}

/* ************************************************************************ */
/*                            GAME INITIALIZATION                           */
/* ************************************************************************ */

/* *** */

/* Tells if the index idx is on the symmetry. */
bool g_index_is_on_symmetry(uint idx) {
    return (WIDTH % 2 == 1) && (idx % WIDTH == WIDTH / 2);
}

/* Get the index of symmetry of idx index. */
int g_get_symmetry_of_idx(uint idx) {
    struct pos pos_act = idx_to_pos(idx);
    struct pos pos_sym = make_pos((WIDTH - 1) - get_x(pos_act), get_y(pos_act));
    return pos_to_idx(pos_sym);
}

enum sort_t get_random_sort(game_config gc) {
    enum sort_t random_sort;

    // As long as the randomly selected sort is not a playable sort, another one is drawn
    do {
        random_sort = get_random_number(MAX_SORT - 1);
    } while (!gc->piece_config[random_sort].play_with);
    return random_sort + 1; // + 1 because the first sort is equal to 1 and not 0
}

/* *** */

/* Defines the initial positions of the parts of a game according to the defined configuration */
void g_init_ip(game g, game_config gc, struct ens init_positions) {
    for (uint piece_id = 0; piece_id < HEIGHT; piece_id++) {
        uint random_id, random_sym_id;
        enum sort_t random_sort = NO_SORT;

        // if not init pos defined, ramdom generate
        if (ens_get_val(init_positions, piece_id) == UINT_MAX) {
            do {
                random_id = get_random_number(WORLD_SIZE);
            } while (ens_found_idx(g->gs.initial_positions, random_id, 2 * HEIGHT) != UINT_MAX || g_index_is_on_symmetry(random_id));
        } else { // if user define init pos
            random_id = ens_get_val(init_positions, piece_id);
            random_sort = ens_get_sort(init_positions, piece_id);
        }

        if (random_sort == NO_SORT) // if no sort defined (by user)
            random_sort = get_random_sort(gc);
        random_sym_id = g_get_symmetry_of_idx(random_id);

        // Set the initial and current positions of the pieces for all users
        ens_set_val(&g->gs.initial_positions, piece_id, random_id);
        ens_set_val(&g->gs.current_positions, piece_id, random_id);
        ens_set_val(&g->gs.initial_positions, HEIGHT + piece_id, random_sym_id);
        ens_set_val(&g->gs.current_positions, HEIGHT + piece_id, random_sym_id);

        // Set the colors and sorts of the pieces in the world
        g_set_color(g, random_id, WHITE);
        g_set_color(g, random_sym_id, BLACK);
        g_set_sort(g, random_id, random_sort);
        g_set_sort(g, random_sym_id, random_sort);
    }
}

void game_init_parameters(game g, game_config gc) {
    g->gp.change_random_seed = gc->game_param.change_random_seed;
    g->gp.game_mode = gc->game_param.game_mode;
    g->gp.max_turns = gc->game_param.max_turns;
    g->gp.play_with_prisoner = gc->game_param.play_with_prisoner;
    g->gs.current_player = gc->game_param.initial_player;

    g_init_ip(g, gc, gc->initial_positions); // init initial positions
}

void game_init_status(game g) {
    g->gs.initial_positions = ens_create(UINT_MAX);
    g->gs.current_positions = ens_create(UINT_MAX);
    g->gs.current_prisoners = ens_create(false);

    g->gs.nb_turns = 0;
    g->gs.winning_status = NO_WIN;
}

/* Auxiliary function of game_init_move_scores */
void g_aux_init_sip(game g, uint idx, uint player, uint score) {
    uint current_score = ens_get_val(g->score_init_pos[player], idx);

    // If the score that can be defined is better than the one already defined
    if (current_score == UINT_MAX || current_score > score) {
        ens_set_val(&g->score_init_pos[player], idx, score); // Set the new score

        // Call the same function on neighbors with a score += 1
        struct neighbors_t n = get_neighbors(idx);
        for (uint i = 0; IDX(n) != UINT_MAX; i++)
            g_aux_init_sip(g, IDX(n), player, score + 1);
    }
}

void game_init_move_scores(game g) {
    // Initialize all scores to ENS_INITIALIZATION_VALUE
    for (uint player = 0; player < MAX_PLAYERS; player++)
        g->score_init_pos[player] = ens_create(UINT_MAX);

    // For each initial position, we set the score of the cell to 0 (step 1)
    // Then apply our algorithm for calculating the score on the neighbours. (step 2)
    // Example on basic game. X is an initial position
    //
    // X _ _ _       0 1 2 3
    // _ _ _ _  -->  1 1 2 3
    // _ X _ _       1 0 1 2

    for (uint i = 0; i < HEIGHT; i++)
        for (uint player = 0; player < MAX_PLAYERS; player++) {
            uint id_initial_position = ens_get_val(g->gs.initial_positions, i + player * HEIGHT);
            ens_set_val(&g->score_init_pos[player], id_initial_position, 0); // step 1

            struct neighbors_t n = get_neighbors(id_initial_position);
            for (uint i = 0; n.n[i].i != UINT_MAX; i++)
                g_aux_init_sip(g, n.n[i].i, player, 1); // step 2
        }
}

void game_init_moves(game_config gc) {
    for (enum sort_t sort = 1; sort < MAX_SORT; sort++)
        define_moves_per_sort(sort, gc->piece_config[sort - 1].m);
}

/* ************************************************************************ */
/*                              LOG FUNCTIONS                               */
/* ************************************************************************ */

/* Do n line jump */
void line_jmp(uint n) {
    if (!n) return;
    printf("\n");
    line_jmp(--n);
}

/* Make a line of length size */
void h_line(uint size) {
    if (!size) return;
    printf("-");
    h_line(--size);
}

void g_display(cgame g, struct move_parameters mp) {
    h_line(4 * WIDTH + 1);
    line_jmp(1);
    for (uint y = 0; y < HEIGHT; y++) {
        for (uint x = 0; x < WIDTH; x++) {
            // For each position on the grid :
            // - Print the sort on blue color if (x, y) is the selected sort
            // - Print the sort on green color if (x, y) is the destination sort
            // - Print the sort on red color if (x, y) is a capture
            // - Else, print normal

            uint idx = pos_to_idx(make_pos(x, y));
            printf("|");
            if (idx == mp.id_src)
                printf(COLOR_BLUE);
            else if (idx == mp.id_dst)
                printf(COLOR_GREEN);
            else if (ens_get_val(*mp.moves, idx) == MOVE_POSSIBLE || ens_get_val(*mp.moves, idx) == MOVE_WITH_CAPTURE)
                printf(COLOR_RED);
            printf(" %s " COLOR_RESET, place_to_string(game_get_color(g, idx), game_get_sort(g, idx)));
        }
        printf("|");
        line_jmp(1);
        h_line(4 * WIDTH + 1);
        line_jmp(1);
    }
}

void log_chosen_move(struct move_parameters mp) {
    printf(">> Chosen piece %u", mp.id_src);
}

void log_player(struct move_parameters mp) {
    printf(">> Player %d plays", mp.current_player);
}

void log_move_details(struct move_parameters mp) {
    printf(">> ");
    switch (mp.move) {
        case MOVE_POSSIBLE:
            printf("Move from %u to %u", mp.id_src, mp.id_dst);
            break;
        case MOVE_NOT_POSSIBLE:
            printf("No move possible");
            break;
        case ESCAPE:
            printf("Escape succeeded");
            break;
        case ESCAPE_FAILED:
            printf("Escape failed");
            break;
        case MOVE_WITH_CAPTURE:
            printf("Capture piece at %u", mp.id_dst);
            break;

        default: break;
    }
}

void log_change_seed() {
    printf(">> Seed changed for seed %d", get_neighbors_seed());
}

void game_display_logs(cgame g, struct move_parameters mp, bool seed_changed) {
    log_player(mp); // Player
    line_jmp(1);

    log_chosen_move(mp); // Chosen move
    line_jmp(2);

    g_display(g, mp); // Grid display
    line_jmp(1);

    log_move_details(mp); // Move details
    line_jmp(2);

    if (!game_is_over(g))
        printf("----- NEXT MOVE -----");
    else
        printf("------ ENDGAME ------");
    line_jmp(2);

    if (seed_changed && !game_is_over(g)) { // Seed have changed
        h_line(4 * WIDTH + 1);
        line_jmp(2);
        log_change_seed();
        line_jmp(2);
        h_line(4 * WIDTH + 1);
        line_jmp(2);
    }
}
