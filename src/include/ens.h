#ifndef __SET_H__
#define __SET_H__

#include "geometry.h"
#include "utils.h"

struct ens {
    struct place_t {
        uint val; /* value associated to the place*/
        enum sort_t sort; /* sort of the place */
        enum color_t color; /* color of the place */
    } place[WORLD_SIZE]; /* list of all the places contained in ens */
};

/**
 * @brief Sets all parameters of a place indexed idx in an ens structure.
 *
 * @param e ens structure
 * @param idx index of place
 * @param val value to set
 * @param c color to set
 * @param s sort to set
 * @pre @p e must be a valid pointer to a ens structure
 * @pre @p color < MAX_COLOR
 * @pre @p sort < MAX_SORT
 */
void ens_fill(struct ens* e, uint idx, uint val, enum color_t c, enum sort_t s);

/**
 * @brief Creates a struct ens with each places initialized with val, NO_SORT and NO_COLOR
 *
 * @param val wanted initialized value
 * @return struct ens
 */
struct ens ens_create(uint val);

/**
 * @brief Search for the index of the place in e containing the value idx.
 * Search between the indexes r_inf and r_sup.
 *
 * @param e ens structure you want to search in
 * @param idx value you want to search or
 * @param r_inf lower boundary where you want to start searching
 * @param r_sup upper boundary where you want to stop searching
 * @return index of the place containing idx
 * @return and UINT_MAX if idx is not found
 */
uint ens_found_range(struct ens e, uint idx, uint r_inf, uint r_sup);

/**
 * @brief Search for the index of the place in e containing the value idx.
 * Search between the indexes 0 and size.
 *
 * @param e ens structure you want to search in
 * @param idx value you want to search for
 * @param size value between 0 and WORLD_SIZE
 * @return index of the place containing idx
 * @return and UINT_MAX if idx is not found
 */
uint ens_found_idx(struct ens e, uint idx, int size);

/**
 * @brief Set a new value v for the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index
 * @param val value
 * @pre @p e must be a valid pointer to a ens structure
 * @pre @p idx < WORLD_SIZE
 */
void ens_set_val(struct ens* e, uint idx, uint val);

/**
 * @brief Set a new sort for the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index
 * @param sort sort
 * @pre @p e must be a valid pointer to a ens structure
 * @pre @p idx < WORLD_SIZE
 * @pre @p sort < MAX_SORT
 */
void ens_set_sort(struct ens* e, uint idx, enum sort_t sort);

/**
 * @brief Set a new color for the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index
 * @param color color
 * @pre @p e must be a valid pointer to a ens structure
 * @pre @p idx < WORLD_SIZE
 * @pre @p color < MAX_COLOR
 */
void ens_set_color(struct ens* e, uint idx, enum color_t color);

/**
 * @brief Return the value of the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index of the place
 * @pre @p idx < WORLD_SIZE
 * @return value of the place
 */
uint ens_get_val(struct ens e, uint idx);

/**
 * @brief Return the sort of the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index of the place
 * @pre @p idx < WORLD_SIZE
 * @return sort of the place
 */
enum sort_t ens_get_sort(struct ens e, uint idx);

/**
 * @brief Return the color of the place indexed idx in e.
 *
 * @param e struct ens
 * @param idx index of the place
 * @pre @p idx < WORLD_SIZE
 * @return color of the place
 */
enum color_t ens_get_color(struct ens e, uint idx);

#endif // __SET_H__
