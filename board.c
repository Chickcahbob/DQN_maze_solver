#include "board.h"

enum board_location* create_board( int width, int height){

    enum board_location* board = (enum board_location*)malloc( width * height * sizeof(enum board_location));

    int cur_location;
    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){
            cur_location = get_position( x, y, width );
            
            *(board + cur_location) = _EMPTY;

        }
    }

    set_holes( board, width, height );
    set_objective( board, width, height, _OBJECTIVE );
    set_objective( board, width, height, _AGENT );
    return board;

}

void delete_board( enum board_location* board ){

    if( board != NULL ){
        free( board );
        board = NULL;
    }:

}

void print_board( enum board_location* board, int width, int height ){

    int cur_location;

    char enum_to_char;

    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){

            cur_location = get_position( x, y, width );

            switch( *(board + cur_location) ){

                case _AGENT:
                    enum_to_char = 'I';
                    break;
                case _OBJECTIVE:
                    enum_to_char = 'O';
                    break;
                case _HOLE:
                    enum_to_char = 'H';
                    break;
                default:
                    enum_to_char = '_';
                    break;

            }

            fprintf( stdout, "%c",  enum_to_char );
        }

        fprintf( stdout, "\n" );

    }

}

int num_values_near( enum board_location* board, int width, int height, int location, enum board_location search_value ){

    int found_values = 0;
    int y_min, y_max, x_min, x_max, cur_search;

    char cur_value;

    struct coords coordinates = get_coordinates( location, width );

    if( coordinates.y == 0 ){
        
        y_min = 0;
        y_max = coordinates.y + 1;

    } else if( coordinates.y == height - 1 ){

        y_min = coordinates.y - 1;
        y_max = height - 1;

    } else {

        y_min = coordinates.y - 1;
        y_max = coordinates.y + 1;

    }

    if( coordinates.x == 0 ){
        
        x_min = 0;
        x_max = coordinates.x + 1;

    } else if( coordinates.x == width - 1 ){

        x_min = coordinates.x - 1;
        x_max = width - 1;

    } else {

        x_min = coordinates.x - 1;
        x_max = coordinates.x + 1;

    }

    //fprintf( stdout, "Location x = %d, x_min = %d, x_max = %d\n", location_x, x_min, x_max );
    //fprintf( stdout, "Location y = %d, y_min = %d, y_max = %d\n", location_y, y_min, y_max );

    for( int y = y_min; y <= y_max; y++ ){

        for( int x = x_min; x <=x_max; x++ ){

            cur_search = get_position( x, y, width );
            cur_value = get_value( board, x, y, width );

            //fprintf( stdout, "Cur location = %d\n", cur_search );
            
            if( cur_value == search_value && cur_search != location ){
                found_values++;
                //set_char(board, i, j, x, 's' );
            }

        }

    }

    return found_values;


}

enum board_location get_value(enum board_location* board, int x, int y, int width){

    enum board_location value = *(board + get_position(x, y, width) );

    return value;

}

void set_value( enum board_location* board, int x, int y, int width, enum board_location value){

    *(board + get_position( x, y, width ) ) = value;

}

void set_holes(enum board_location *board, int width, int height){

    int max_holes, holes_near, make_hole;

    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){

            max_holes = 2;

            if( y == 0 || y == height - 1 ){
                max_holes--;
            }

            if( x == 0 || x == width - 1 ){
                max_holes--;
            }

            holes_near = num_values_near(board, width, height, get_position( x, y, width), _HOLE);

            if( holes_near < max_holes ){

                make_hole = rand() % max_holes;

                if( make_hole == 0 ){

                    set_value( board, x, y, width, _HOLE );

                }


            }

        }

    }

}

void set_objective( enum board_location* board, int x, int y, enum board_location type){

    int location_x = rand() % x;
    int location_y = rand() % y;

    char cur_value = get_value(board, location_x, location_y, x);

    int objectives_near = num_values_near(board, x, y, location_y * x + location_x, 'O');

    while( cur_value == _OBJECTIVE || cur_value == _AGENT || objectives_near != 0 ){

        location_x = rand() % x;
        location_y = rand() % y;

        cur_value = get_value(board, location_x, location_y, x);
        objectives_near = num_values_near(board, x, y, location_y * x + location_x, _OBJECTIVE );

    }

    set_value( board, location_x, location_y, x, type );

}

struct coords get_coordinates(int position, int width){

    struct coords coordinates;

    coordinates.x = position % width;
    coordinates.y = position / width;

    return coordinates;

}

int get_position(int x, int y, int width){

    int position = y * width + x;

}
