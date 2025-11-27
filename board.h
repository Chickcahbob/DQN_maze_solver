#ifndef BOARD_H
#define BOARD_H

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

enum board_location{

    _AGENT,
    _HOLE,
    _OBJECTIVE,
    _EMPTY

};

struct coords{
    int x;
    int y;
};

struct coords_ll{

    struct coords values;
    struct coords_ll* next;

};

struct directions{

    bool left;
    bool right;
    bool up;
    bool down;

};

enum board_location* create_board( int width, int height );

int save_board( enum board_location** backup_board, enum board_location* board, int width, int height);

void delete_board( enum board_location** board );

void print_board( enum board_location* board, int width, int height );

int generate_tree( enum board_location* board, int height, int width);

int num_values_near( enum board_location* board, int width, int height, int location, enum board_location search_value );

struct coords_ll* initialize_coords();

struct directions check_paths( struct coords node, int height, int width, enum board_location* board);

void create_bridge( struct coords node, enum board_location* board, int height, int width, int direction );
bool check_bridge( struct coords node, enum board_location* board, int height, int width );

int add_coords_ll( struct coords_ll* head, struct coords_ll* coords_to_add );

struct coords select_coords_ll( struct coords_ll* head, int index );

void delete_coords( struct coords_ll* head, int index );

void delete_coords_ll( struct coords_ll** head );

enum board_location get_value( enum board_location* board, int x, int y, int width );

void set_value( enum board_location* board, int x, int y, int width, enum board_location value );

struct coords get_coordinates( int position, int width );

int get_position( int x, int y, int width );

#endif
