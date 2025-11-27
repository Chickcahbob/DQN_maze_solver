#include "board.h"

enum board_location* create_board( int width, int height){

    assert( height % 2 == 1 );
    assert( width % 2 == 1 );

    enum board_location* board = (enum board_location*)malloc( width * height * sizeof(enum board_location));

    int cur_location;
    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){
            cur_location = get_position( x, y, width );
            
            *(board + cur_location) = _HOLE;

        }
    }

    int tree_test = generate_tree( board, height, width);
    assert( tree_test == 0 );
    return board;

}

int generate_tree( enum board_location* board, int height, int width){

    int board_size = height * width;
    int success = 1;

    int num_x = ( width + 1 ) / 2;
    int num_y = ( height + 1 ) / 2;

    int num_nodes = num_x * num_y;

    struct coords_ll* head = initialize_coords();

    head->values.x = (rand()  % num_x) * 2;
    head->values.y = (rand()  % num_y) * 2;

    set_value(board, head->values.x, head->values.y, width, _EMPTY);

    struct coords_ll* next = initialize_coords();

    next->values.x = (rand()  % num_x) * 2;
    next->values.y = (rand()  % num_y) * 2;

    set_value(board, next->values.x, next->values.y, width, _EMPTY);

    delete_coords_ll(&head);

    success = 0;
    assert( head == NULL );

    return success;
}

struct coords select_coords_ll( struct coords_ll* head, int index ){

    struct coords_ll* iterator = head;

    for( int i = 0; i < index; i++ ){

        iterator = iterator->next;

    }
    
    return iterator->values;

};

void delete_coords( struct coords_ll* head, int index ){

    assert( index != 0 );

    struct coords_ll* iterator = head;
    struct coords_ll* iterator_next;

    for( int i = 0; i < index - 1; i++ ){

        iterator = iterator->next;

    }

    iterator_next = iterator->next->next;

    iterator->next = iterator_next;


}

void delete_coords_ll( struct coords_ll** head ){

    struct coords_ll* iterator = (*head);

    while( iterator->next != NULL ){
        delete_coords((*head),1);
    }


    free( (*head) );

    (*head) = NULL;


}

void add_coords_ll( struct coords_ll* head, struct coords_ll* coords_to_add ){

    struct coords_ll* iterator = head;

    while( iterator->next != NULL ){

        iterator = iterator->next;

    }

    iterator->next = coords_to_add;

}

struct coords_ll* initialize_coords(){

    struct coords_ll* new_coords_ll = (struct coords_ll*) malloc (sizeof( struct coords_ll ) );

    return new_coords_ll;

}

int save_board( enum board_location** backup_board, enum board_location* board, int width, int height){

    int success = 1;
    enum board_location tmp;
    (*backup_board) = (enum board_location*) malloc( sizeof( enum board_location) * width * height );

    for( int y = 0; y < height; y++ ){
        for( int x = 0; x < width; x++ ){
            tmp = get_value(board, x, y, width);
            set_value((*backup_board), x, y, width, tmp);

        }
    }

    if( backup_board != NULL )
        success = 0;

    return success;
}

void delete_board( enum board_location** board ){

    if( (*board) != NULL ){
        free( (*board) );
        (*board) = NULL;
    }

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


struct coords get_coordinates(int position, int width){

    struct coords coordinates;

    coordinates.x = position % width;
    coordinates.y = position / width;

    return coordinates;

}

int get_position(int x, int y, int width){

    int position = y * width + x;

    return position;
}
