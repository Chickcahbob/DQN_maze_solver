#ifndef BOARD_H
#define BOARD_H

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

struct coords{
    int x;
    int y;
};

char* create_board( int width, int height );

void delete_board( char* board );

void print_board( char* board, int width, int height );

int num_chars_near( char* board, int width, int height, int location, char search_char );

char get_char( char* board, int x, int y, int width );

void set_char( char* board, int x, int y, int width, char value );

void set_holes( char* board, int width, int height );

void set_objective( char* board, int width, int height, char type );

struct coords get_coordinates( int position, int width );

int get_position( int x, int y, int width );

#endif
