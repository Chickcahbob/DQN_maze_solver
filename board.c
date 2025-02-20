#include "board.h"

char* create_board( int width, int height){

    char* board = (char*)malloc( width * height * sizeof(char));

    fprintf( stdout, "Board created at %p\n", (void*)board );

    int cur_location;
    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){
            cur_location = get_position( x, y, width );
            
            *(board + cur_location) = '_';

        }
    }

    set_holes( board, width, height );
    set_objective( board, width, height, 'O' );
    set_objective( board, width, height, 'I' );
    return board;

}

void delete_board( char* board ){

    free( board );

}

void print_board( char* board, int width, int height ){

    int cur_location;

    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){

            cur_location = get_position( x, y, width );

            fprintf( stdout, "%c", *(board + cur_location ) );
        }

        fprintf( stdout, "\n" );

    }

}

int num_chars_near( char* board, int width, int height, int location, char search_char ){

    int found_chars = 0;
    int y_min, y_max, x_min, x_max, cur_search;

    char cur_char;

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
            cur_char = get_char( board, x, y, width );

            //fprintf( stdout, "Cur location = %d\n", cur_search );
            
            if( cur_char == search_char && cur_search != location ){
                found_chars++;
                //set_char(board, i, j, x, 's' );
            }

        }

    }

    return found_chars;


}

char get_char(char *board, int x, int y, int width){

    char value = *(board + get_position(x, y, width) );

    return value;

}

void set_char( char* board, int x, int y, int width, char value){

    *(board + get_position( x, y, width ) ) = value;

}

void set_holes(char *board, int width, int height){

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

            holes_near = num_chars_near(board, width, height, get_position( x, y, width), 'H');

            if( holes_near < max_holes ){

                make_hole = rand() % max_holes;

                if( make_hole == 0 ){

                    set_char( board, x, y, width, 'H' );

                }


            }

        }

    }

}

void set_objective( char* board, int x, int y, char type){

    int location_x = rand() % x;
    int location_y = rand() % y;

    char cur_value = get_char(board, location_x, location_y, x);

    int objectives_near = num_chars_near(board, x, y, location_y * x + location_x, 'O');

    while( cur_value == 'O' || cur_value == 'I' || objectives_near != 0 ){

        location_x = rand() % x;
        location_y = rand() % y;

        cur_value = get_char(board, location_x, location_y, x);
        objectives_near = num_chars_near(board, x, y, location_y * x + location_x, 'O');

    }

    set_char( board, location_x, location_y, x, type );

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
