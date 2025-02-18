#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "board.h"
#include "keyboard.h"

char move( char* board, int width, int height, char stored_char);

char move_up( char* board, int width, int height, char stored_char );
char move_down( char* board, int width, int height, char stored_char );
char move_right( char* board, int width, int height, char stored_char );
char move_left( char* board, int width, int height, char stored_char );

int get_agent_position( char* board, int width, int height );

#endif
