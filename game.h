#ifndef PLAY_H
#define PLAY_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "board.h"
#include "keyboard.h"
#include "movement.h"
#include "dqn.h"


int user_play( int width, int height );

int ai_play( int width, int height );

enum board_location selection_translate( enum board_location* board, enum board_location stored_value, int height, int width, int action );

#endif
