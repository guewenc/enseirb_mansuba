#ifndef __NEIGHBORS_SEED_H__
#define __NEIGHBORS_SEED_H__

#include "geometry.h"
#include "utils.h"

/**
 * @brief Gets the neighbor place of index idx following direction d, based on the actual seed.
 *
 * @param seed valid seed
 * @param idx valid idx
 * @param d valid direction in given seed
 * @return index of neighbor or
 * UINT_MAX if the translation or the seed are not valid.
 */
uint get_neighbor_of_seed(uint seed, uint idx, enum dir_t d);

#endif // __NEIGHBORS_SEED_H__
