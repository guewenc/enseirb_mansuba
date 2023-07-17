#include "include/tst_geometry.h"
#include <limits.h>
#include "geometry.h"

struct test tests_geometry[] = {
    {tst_place_to_string, "place_to_string"},
    {tst_dir_to_string, "dir_to_string"},
};

tests tst_geometry_list() {
    return create_tests(tests_geometry, TST_COUNT_GEOMETRY);
}

// Here, we simply call the functions to ensure that they do not cause a
// segmentation fault. What they return is not particularly important.

void tst_place_to_string() {
    place_to_string(WHITE, PAWN);
    place_to_string(BLACK, PAWN);
    place_to_string(WHITE, ELEPHANT);
    place_to_string(WHITE, TOWER);
    place_to_string(WHITE, NO_SORT);
    place_to_string(NO_COLOR, PAWN);
}

void tst_dir_to_string() {
    dir_to_string(NO_DIR);
    dir_to_string(EAST);
    dir_to_string(NEAST);
    dir_to_string(NORTH);
    dir_to_string(SOUTH);
    dir_to_string(-10);
}