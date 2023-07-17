#ifndef __TST_GAME_H__
#define __TST_GAME_H__

#include "utils.h"

#define TST_COUNT_GAME 10

tests tst_game_list();

void tst_game_new();
void tst_game_next_player();
void tst_game_get_current_player();
void tst_game_play_move();
void tst_game_is_over();
void tst_game_play_with_prisoner();
void tst_game_get_color();
void tst_game_get_sort();
void tst_game_get_win_status();
void tst_game_delete();

#endif