#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void flush_input( char user_input[] );

char get_direction();

#endif
