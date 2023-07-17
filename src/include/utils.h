#ifndef __UTILS_H__
#define __UTILS_H__

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDX(n) n.n[i].i
#define DIR(n) n.n[i].d

/**
 * @brief Standard unsigned integer type.
 */
typedef unsigned int uint;

/**
 * @brief Get a random number between 0 and range (not included).
 *
 * @param range range
 * @return random uint number less than range
 */
int get_random_number(int range);

/**
 * @brief Checks if an index is valid (i.e. comprised between 0 and WORLD_SIZE not included).
 *
 * @param idx index
 * @return true if index valid and
 * @return false otherwise
 */
bool is_valid_idx(uint idx);

#endif // __UTILS_H__
