#ifndef __GAME_CONFIGURATION_H__
#define __GAME_CONFIGURATION_H__

#include "game.h"
#include "movement.h"
#include "utils.h"

/**
 * @brief Configuration of a piece
 */
struct piece_config {
    bool play_with; /* Boolean value telling if a piece will be used or not */
    movement m[MAX_MOVE_TYPES]; /* Movements authorized for said piece */
};

/**
 * @brief Game parameters
 */
struct game_param {
    uint max_turns; /* Maximum turns of the game */
    bool play_with_prisoner; /* Prisoner mode */
    enum game_mode game_mode; /* Game mode */
    enum grid_mode grid_mode; /* Type of game board */
    bool change_random_seed; /* Changing game board mode */
    uint initial_player; /* Defines which player will play first */
};

/**
 * @brief All parameters of the entire game configuration
 */
struct game_config {
    struct piece_config piece_config[MAX_SORT]; /* List of pieces parameters */
    struct game_param game_param; /* Parameters of the game */
    struct ens initial_positions; /* Initial positions on starting game */
};

/**
 * @brief Sets the maximum number of turn for the game initial configuration.
 *
 * @param gc game configuration
 * @param max_turns value of the maximum number of turns
 */
void game_configuration_set_max_turns(game_config gc, uint max_turns);

/**
 * @brief Sets prisoner mode for the game initial configuration.
 * If prisoner mode is set to True, capture moves will be possible.
 *
 * @param gc game configuration
 * @param play_with_prisoner boolean value
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_play_with_prisoner(game_config gc, bool play_with_prisoner);

/**
 * @brief Sets type of win for the game initial configuration.
 *
 * @param gc game configuration
 * @param game_mode type of win chosen (SIMPLE_WIN or COMPLEX_WIN)
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_game_mode(game_config gc, enum game_mode game_mode);

/**
 * @brief Sets the type of board for the game initial configuration.
 *
 * @param gc game configuration
 * @param grid_mode type of board game chosen
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_grid_mode(game_config gc, enum grid_mode grid_mode);

/**
 * @brief Sets the changing board mode for the game initial configuration.
 * Changing board mode makes the board type sometime change while the game is playing.
 *
 * @param gc game configuration
 * @param change_seed boolean value
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_change_seed(game_config gc, bool change_seed);

/**
 * @brief Sets initial player for the game initial configuration.
 *
 * @param gc game configuration
 * @param initial_player first player to play
 * @pre @p gc must be a valid pointer toward a game_config structure.
 * @pre @p initial_player must be >= -1 and < MAX_PLAYERS
 */
void game_configuration_set_initial_player(game_config gc, uint initial_player);

/**
 * @brief Sets initial positions of pieces for the game initial configuration.
 *
 * @param gc game configuration
 * @param init_positions ens structure
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_init_positions(game_config gc, struct ens init_positions);

/**
 * @brief Enable a certain piece in the game configuration.
 *
 * @param gc game configuration
 * @param sort sort to enable
 * @pre @p gc must be a valid pointer toward a game_config structure.
 * @pre @p sort must be > 0 and < MAX_SORT
 */
void game_configuration_enable_sort(game_config gc, enum sort_t sort);

/**
 * @brief Enable all pieces in the game configuration.
 *
 * @param gc game configuration
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_enable_all_sort(game_config gc);

/**
 * @brief Disable a certain piece in the game configuration.
 *
 * @param gc game configuration
 * @param sort sort to disable
 * @pre @p gc must be a valid pointer toward a game_config structure.
 * @pre @p sort must be > 0 and < MAX_SORT
 */
void game_configuration_disable_sort(game_config gc, enum sort_t sort);

/**
 * @brief Disable all pieces in the game configuration.
 *
 * @param gc game configuration
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_disable_all_sort(game_config gc);

/**
 * @brief Sets movements for a given piece in game configuration.
 *
 * @param gc game configuration
 * @param sort type of piece to configure
 * @param m list of authorized movements
 * @pre @p gc must be a valid pointer toward a game_config structure.
 * @pre @p sort must be > 0 and < MAX_SORT
 */
void game_configuration_set_movement(game_config gc, enum sort_t sort, movement m[MAX_MOVE_TYPES]);

/**
 * @brief Sets movements for all pieces in game configuration.
 *
 * @param gc The game configuration to set the movements for
 * @param m The array of movements to set for each sort of piece
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
void game_configuration_set_all_movement(game_config gc, movement m[MAX_SORT - 1][MAX_MOVE_TYPES]);

/**
 * @brief Creates a new structure game configuration, using base values to initialize parameters :
 * max_turns is set to 2 * WORLD_SIZE, initial player is randomly chosen and initial positions are set.
 *
 * @return game configuration
 */
game_config game_configuration_new_empty();

/**
 * @brief Returns the game configuration using a preset configuration chosen with id_config. An invalid id will get the base configuration.
 *
 * @param id_config id of the preset configuration
 * @return game configuration
 */
game_config game_configuration_get(int id_config);

/**
 * @brief Free the allocation of the game_config structure.
 *
 * @param gc game configuration
 */
void game_configuration_delete(game_config gc);

#endif // __GAME_CONFIGURATION_H__
