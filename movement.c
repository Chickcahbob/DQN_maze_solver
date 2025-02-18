#include "movement.h"
#include "board.h"

char move( char* board, int width, int height, char stored_char){

    char movement_direction = get_direction();
    char next_stored_char = '0';

    if( movement_direction == '0' )
        return next_stored_char;

    switch( movement_direction ){

        case 'A':
            next_stored_char = move_up(board, width, height, stored_char);
            break;

        case 'B':
            next_stored_char = move_down(board, width, height, stored_char);
            break;

        case 'C':
            next_stored_char = move_right(board, width, height, stored_char);
            break;
        
        case 'D':
            next_stored_char = move_left(board, width, height, stored_char);
            break;

    }

    return next_stored_char;

}

char move_up( char* board, int width, int height, char stored_char ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.y == 0 )
        return '0';

    char new_stored_char = get_char(board, agent_coords.x, agent_coords.y - 1, width);

    set_char(board, agent_coords.x, agent_coords.y - 1, width, 'I');

    if( stored_char != '0' )
        set_char( board, agent_coords.x, agent_coords.y, width, stored_char );
    else
        set_char( board, agent_coords.x, agent_coords.y, width, '_' );

    return new_stored_char;
    
}

char move_down( char* board, int width, int height, char stored_char ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.y == height - 1 )
        return '0';

    char new_stored_char = get_char(board, agent_coords.x, agent_coords.y + 1, width);

    set_char(board, agent_coords.x, agent_coords.y + 1, width, 'I');

    if( stored_char != '0' )
        set_char( board, agent_coords.x, agent_coords.y, width, stored_char );
    else
        set_char( board, agent_coords.x, agent_coords.y, width, '_' );

    return new_stored_char;
    
}

char move_right( char* board, int width, int height, char stored_char ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.x == width - 1 )
        return '0';

    char new_stored_char = get_char(board, agent_coords.x + 1, agent_coords.y, width);

    set_char(board, agent_coords.x + 1, agent_coords.y, width, 'I');

    if( stored_char != '0' )
        set_char( board, agent_coords.x, agent_coords.y, width, stored_char );
    else
        set_char( board, agent_coords.x, agent_coords.y, width, '_' );

    return new_stored_char;
    
}

char move_left( char* board, int width, int height, char stored_char ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.x == 0 )
        return '0';

    char new_stored_char = get_char(board, agent_coords.x - 1, agent_coords.y, width);

    set_char(board, agent_coords.x - 1, agent_coords.y, width, 'I');

    if( stored_char != '0' )
        set_char( board, agent_coords.x, agent_coords.y, width, stored_char );
    else
        set_char( board, agent_coords.x, agent_coords.y, width, '_' );

    return new_stored_char;
    
}



int get_agent_position( char* board, int width, int height ){

    int agent_position = 0;
    bool agent_found = false;

    int x = 0;
    int y = 0;

    while( agent_found == false ){

        if( get_char(board, x, y, width) == 'I'){

            agent_found = true;

            agent_position = get_position( x, y, width );

        } else {

            x++;

            if( x == width ){
                y++;
                x = 0;
            }

        }

    }

    return agent_position;

}
