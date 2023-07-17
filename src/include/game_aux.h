#ifndef __GAME_AUX_H__
#define __GAME_AUX_H__

#include "game.h"
#include "game_config.h"

struct move_parameters {
    enum move_t move; /* Move chosen by the moving piece */
    uint id_src; /* Index of the position of moving piece */
    uint id_dst; /* Index of destination of the moving piece */
    struct ens* moves; /* List of all moves possible */
    uint current_player; /* Player currently moving a piece */
};

struct game_parameters {
    uint max_turns; /* Maximum turns of the game (choise or random) */
    bool play_with_prisoner; /* Prisoner mode */
    enum game_mode game_mode; /* Game mode (choise or random) */
    bool change_random_seed; /* Changing seed mode */
};

struct game_status {
    struct ens initial_positions; /* Initial positions on starting game */
    struct ens current_positions; /* Current positions on playing game */
    struct ens current_prisoners; /* List of prisoners sorts */
    uint current_player; /* Player who currently play */
    uint nb_turns; /* Correlation with max_turns parameters */
    enum game_mode winning_status; /* When game is over, give end status */
};

struct game {
    struct game_parameters gp; /* Parameters of the game */
    struct game_status gs; /* Status of the game */
    struct ens score_init_pos[MAX_PLAYERS]; /* Sets defining the distances to the closest enemy's initial position (permitting to find the best moves possible) */
    struct world_t* world; /* Actual state of the game board */
};

/**
 * @brief Sets the color c for the place indexed idx of the game g.
 *
 * @param g game
 * @param idx valid idx
 * @param c color you want to set
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p idx < WORLD_SIZE
 * @pre @p c < MAX_COLOR
 */
void g_set_color(game g, uint idx, enum color_t c);

/**
 * @brief Sets the sort c for the place indexed idx of the game g.
 *
 * @param g game
 * @param idx valid idx
 * @param s sort you want to set
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p idx < WORLD_SIZE
 * @pre @p s < MAX_SORT
 */
void g_set_sort(game g, uint idx, enum sort_t s);

/* ************************************************************************ */
/*                            GAME INITIALIZATION                           */
/* ************************************************************************ */

/**
 * @brief Initializes the game g by setting all parameters of game g at parameters given by gc.
 * Also creates the board according to given parameters, and fills up initial positions of pieces if not given.
 *
 * @param g game
 * @param gc game_config that will define future parameters of the game
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_init_parameters(game g, game_config gc);

/**
 * @brief Initializes status of the game g by initializing all sets at their initial values,
 * setting the turn count to 0 and setting win status to NO_WIN.
 *
 * @param g game
 * @pre @p g must be a valid pointer toward a game structure.
 */
void game_init_status(game g);

/**
 * @brief Initializes the sets permitting to find the best moves possible when moving a piece
 * (move permitting to get the closest of an enemy initial position).
 *
 * @param g game
 * @pre @p g must be a valid pointer toward a game structure.
 */
void game_init_move_scores(game g);

/**
 * @brief Initializes the authorized movements for each piece in game.
 *
 * @param gc game_config that will define future parameters of the game
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_init_moves(game_config gc);

/* ************************************************************************ */
/*                              LOG FUNCTIONS                               */
/* ************************************************************************ */

/**
 * @brief Displays details a turn passing by in the game.
 *
 * @param g game
 * @param mp move parameters of the turn to display
 * @param seed_changed tells if the seed as changed this turn
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p mp must be a valid move_parameters structure
 * @pre @p seed_changed must be a boolean
 */
void game_display_logs(cgame g, struct move_parameters mp, bool seed_changed);

#endif // __GAME_AUX_H__
