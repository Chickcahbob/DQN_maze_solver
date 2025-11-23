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


int play( int height, int width, bool ai );

#endif
