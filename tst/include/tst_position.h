#ifndef __TST_POSITION_H__
#define __TST_POSITION_H__

#include "utils.h"

#define TST_COUNT_POSITION 10

tests tst_position_list();

void tst_get_x();
void tst_get_y();
void tst_make_pos();
void tst_idx_to_pos();
void tst_pos_to_idx();
void tst_is_inside();
void tst_is_inside_pos();
void tst_pos_translate();
void tst_pos_are_equals();
void tst_display_pos();

#endif