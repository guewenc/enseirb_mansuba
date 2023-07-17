#ifndef __GAME_H__
#define __GAME_H__

#include <time.h>

#include "ens.h"
#include "geometry.h"
#include "neighbors.h"
#include "position.h"
#include "utils.h"
#include "world.h"

/**
 * @brief The different types of victory
 */
enum game_mode {
    NO_WIN, /* The game is not over */
    DRAW, /* The game ends in a tie */
    SIMPLE_WIN, /* The game ends with a simple victory */
    COMPLEX_WIN /* The game ends with a complex victory */
};

/**
 * @brief The different types of game grids
 */
enum grid_mode {
    GRID, /* Original grid */
    HEXAGONAL, /* Hexagonal grid */
    TRIANGLE, /* Triangular grid */
    GRID_ALL /* Grid with displacement in all positions */
};

/**
 * @brief The different possible moves
 */
enum move_t {
    NOT_VISITED, /* Box not visited in the process  */
    MOVE_NOT_POSSIBLE, /* Movement is not possible on this square */
    MOVE_POSSIBLE, /* Movement is possible on this square */
    MOVE_WITH_CAPTURE, /* Movement is possible and causes a capture */
    ESCAPE, /* The movement is a successful escape */
    ESCAPE_FAILED /* The movement is an unsuccessful escape */
};

/**
 * @brief The structure pointer that stores the game state.
 */
typedef struct game* game;

/**
 * @brief The structure constant pointer that stores the game state.
 * That means that it is not possible to modify the game using this pointer.
 */
typedef const struct game* cgame;

/**
 * @brief The structure pointer that stores the game configuration.
 */
typedef struct game_config* game_config;

/**
 * @brief Creates a new game with default size and initializes it.
 *
 * @param game_mode Game mode : victory conditions
 * @param max_turns Maximum number of turns before the game stops.
 * max_turns must be -1 to get the default value.
 * @param init_player Choice of the first player to play.
 * initial_player must be -1 to get a random first player.
 * @param play_with_prisoners To play with the prisoners mode.
 * @param init_positions Defines the initial positions of the game.
 * To not define a position, the structure must contain values equal to constant ENS_INITIALISATION_VALUE.
 * @return The created game
 * @pre @p init_player must be >= -1 and < MAX_PLAYERS
 */
game game_new(enum game_mode game_mode, int max_turns, int init_player, bool play_with_prisoners, struct ens init_positions);

/**
 * @brief Creates a new game using the game configuration to initialize all the game parameters.
 *
 * @param gc game configuration
 * @return the created game
 * @pre @p gc must be a valid pointer toward a game_config structure.
 */
game game_new_with_config(game_config gc);

/**
 * @brief Changes to the next player.
 *
 * @param g The game
 * @pre @p g must be a valid pointer toward a game structure.
 */
void game_next_player(game g);

/**
 * @brief Gets the current player
 *
 * @param g The game
 * @return The current player
 * @pre @p g must be a valid pointer toward a game structure.
 */
uint game_get_current_player(cgame g);

/**
 * @brief Makes a random move for the current player according actual game parameters.
 *
 * @param g The game
 * @pre @p g must be a valid pointer toward a game structure.
 */
void game_move_random(game g);

/**
 * @brief Makes a move according to actual game parameters.
 *
 * @param g The game
 * @param id_src The index of source place
 * @param id_dst The index of destination place OR UINT_MAX if an escape is intended
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p id_src < WORLD_SIZE
 * @pre @p id_dst < WORLD_SIZE || UINT_MAX
 */
void game_play_move(game g, uint id_src, uint id_dst);

/**
 * @brief Checks if the game is finished.
 * @details If the game is won or ended in a tie, the result can be get with function game_get_win_status(g).
 *
 * @param g The game
 * @return true if the game is finished
 * @return false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 */
bool game_is_over(cgame g);

/**
 * @brief Checks if the game has the prisoner option.
 *
 * @param g The game
 * @return true if the game has the prisoner option
 * @return false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 */
bool game_play_with_prisoner(cgame g);

/**
 * @brief Checks if the player has an imprisonned piece at the position idx.
 *
 * @param g The game
 * @param idx Index
 * @param player The player
 * @return true if the player has an imprisonned piece at the position idx
 * @return false otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p idx < WORLD_SIZE
 * @pre @p player < MAX_PLAYERS
 */
bool game_is_prisoner(cgame g, uint idx, uint player);

/**
 * @brief Get the color at the given idx position
 *
 * @param g The game
 * @param idx Index
 * @return The color value if idx is a valid index, INT_MAX otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p idx < WORLD_SIZE
 */
enum color_t game_get_color(cgame g, uint idx);

/**
 * @brief Get the sort at the given idx position
 *
 * @param g The game
 * @param idx Index
 * @return The sort value if idx is a valid index, INT_MAX otherwise
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p idx < WORLD_SIZE
 */
enum sort_t game_get_sort(cgame g, uint idx);

/**
 * @brief Gives the winning status of the game.
 *
 * @param g The game
 * @return The win status
 * @pre @p g must be a valid pointer toward a game structure.
 */
enum game_mode game_get_winning_status(cgame g);

/**
 * @brief Deletes the game and frees the allocated memory.
 *
 * @param g The game to delete
 */
void game_delete(game g);

/**
 * @brief Change the game configuration for a preset configuration chosen with id_config. An invalid id will change for the base configuration.
 *
 * @param id_config id of the preset configuration
 * @return game configuration
 */
game_config game_get_configuration(int id_config);

#endif // __GAME_H__
