#ifndef __MOVEMENT_H__
#define __MOVEMENT_H__

#include "game.h"
#include "geometry.h"
#include "utils.h"

#define MAX_MOVE_TYPES 3

/**
 * @brief defines a function pointer
 */
typedef void (*movement)(struct ens*, cgame, uint);

/**
 * @brief Defines the permitted moves for a given piece's type.
 *
 * @param sort type of the piece
 * @param m list of authorized movements for the given piece's type
 */
void define_moves_per_sort(enum sort_t sort, movement m[MAX_MOVE_TYPES]);

/**
 * @brief Adds to the list apm all possible simple moves from index idx.
 * Simple moves are movements toward one neighbor place.
 *
 * @param apm list of the possible moves
 * @param g game
 * @param idx index from where the piece will move
 * @pre @p apm must be a valid pointer to a ens structure
 * @pre @p g must be a valid pointer toward a game structure.
 */
void simple_move(struct ens* apm, cgame g, uint idx);

/**
 * @brief Adds to the list apm all possible simple jumps from index idx.
 * A simple jump is possible when a piece can go over another piece to get to the place behind it.
 *
 * @param apm list of the possible moves
 * @param g game
 * @param idx index from where the piece will move
 * @pre @p apm must be a valid pointer to a ens structure
 * @pre @p g must be a valid pointer toward a game structure.
 */
void simple_jump(struct ens* apm, cgame g, uint idx);

/**
 * @brief Adds to the list apm all possible double jumps from index idx.
 * A double jump is possible when a piece can move toward another place by making two simple moves. It can go through other pieces this way.
 *
 * @param apm list of the possible moves
 * @param g game
 * @param idx index from where the piece will move
 * @pre @p apm must be a valid pointer to a ens structure
 * @pre @p g must be a valid pointer toward a game structure.
 */
void double_jump(struct ens* apm, cgame g, uint idx);

/**
 * @brief Adds to the list apm all possible multiple jumps from index idx.
 * A multiple jump is possible when after a simple jump, the piece can do another simple jump from its new position, leading to two (or more) sucessive jumps.
 * This function will also add simple jumps to the list apm before adding the multiple jumps.
 *
 * @param apm list of the possible moves
 * @param g game
 * @param idx index from where the piece will move
 * @pre @p apm must be a valid pointer to a ens structure
 * @pre @p g must be a valid pointer toward a game structure.
 */
void multiple_jump(struct ens* apm, cgame g, uint idx);

/**
 * @brief Adds to the list apm all possible moves toward a same direction from index idx.
 * Those moves are possible when a piece can travel any number of places in the given permitted directions.
 *
 * @param apm list of the possible moves
 * @param g game
 * @param idx index from where the piece will move
 * @pre @p apm must be a valid pointer to a ens structure
 * @pre @p g must be a valid pointer toward a game structure.
 */
void move_toward_same_dir(struct ens* apm, cgame g, uint idx);

/**
 * @brief Get the all possible moves for a piece located in place idx.
 *
 * @param g game
 * @param idx index of the piece trying to move
 * @param apm list of all possible moves
 * @pre @p g must be a valid pointer toward a game structure.
 * @pre @p apm must be a valid pointer to a ens structure.
 */
void get_all_possible_moves(cgame g, uint idx, struct ens* apm);

/**
 * @brief Get a random possible move for a given piece.
 *
 * @param g game
 * @param id_src Index of the chosen piece.
 * @return The idx of a possible move
 * @return and UINT_MAX if no move is possible
 * @pre @p g must be a valid pointer toward a game structure.
 */
uint get_random_movement(cgame g, uint id_src);

#endif // __MOVEMENT_H__
