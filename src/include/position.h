#ifndef __POSITION_H__
#define __POSITION_H__

#include "geometry.h"
#include "utils.h"

struct pos {
    int x, y;
};

/**
 * @brief Get the x object
 *
 * @param p Position structure
 * @return x composant of position p
 */
int get_x(struct pos p);

/**
 * @brief Get the y object
 *
 * @param p Position structure
 * @return y composant of position p
 */
int get_y(struct pos p);

/**
 * @brief Create a position in 2D plan
 *
 * @param x x composant of the position
 * @param y y composant of the position
 * @return Position
 */
struct pos make_pos(int x, int y);

/**
 * @brief Get the position of the index idx in 2D plan
 *
 * @param idx A valid index on the grid
 * @return Position
 */
struct pos idx_to_pos(int idx);

/**
 * @brief Get a valid index of position p in 2D plan
 *
 * @param p A position in the game
 * @return Index
 */
int pos_to_idx(struct pos p);

/**
 * @brief Says if the index passed in parameters is on the game grid
 *
 * @param idx Index
 * @return true if the index is on the grid
 * @return false otherwise
 */
bool is_inside(int idx);

/**
 * @brief Says if the position passed in parameters is on the game grid
 *
 * @param p Position
 * @return true if the position is on the grid
 * @return false otherwise
 */
bool is_inside_pos(struct pos p);

/**
 * @brief Applies a translation of the position p by t
 *
 * @param p Position
 * @param t Position
 * @return Translate position
 */
struct pos pos_translate(struct pos p, struct pos t);

/**
 * @brief Tells if the both positions are equals
 *
 * @param p1 Position 1
 * @param p2 Position 2
 * @return true if positions are equals
 * @return false otherwise
 */
bool pos_are_equals(struct pos p1, struct pos p2);

/**
 * @brief Print on standard output the position
 */
void display_pos(struct pos p);

#endif // __POSITION_H__
