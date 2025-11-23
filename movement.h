#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "board.h"
#include "keyboard.h"

enum board_location move( enum board_location* board, int width, int height, enum board_location stored_value, char direction);

enum board_location move_up( enum board_location* board, int width, int height, enum board_location stored_value );
enum board_location move_down( enum board_location* board, int width, int height, enum board_location stored_value );
enum board_location move_right( enum board_location* board, int width, int height, enum board_location stored_value );
enum board_location move_left( enum board_location* board, int width, int height, enum board_location stored_value );

int get_agent_position( enum board_location* board, int width, int height );
int play( int height, int width, bool ai );

#endif
