#include "movement.h"
#include "board.h"

enum board_location move( enum board_location* board, int width, int height, enum board_location stored_value){

    char movement_direction = get_direction();
    enum board_location next_stored_value = _EMPTY;

    if( movement_direction == '0' )
        return next_stored_value;

    switch( movement_direction ){

        case 'A':
            next_stored_value = move_up(board, width, height, stored_value);
            break;

        case 'B':
            next_stored_value = move_down(board, width, height, stored_value);
            break;

        case 'C':
            next_stored_value = move_right(board, width, height, stored_value);
            break;
        
        case 'D':
            next_stored_value = move_left(board, width, height, stored_value);
            break;

    }

    return next_stored_value;

}

enum board_location move_up( enum board_location* board, int width, int height, enum board_location stored_value ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.y == 0 )
        return '0';

    enum board_location new_stored_value = get_value(board, agent_coords.x, agent_coords.y - 1, width);

    set_value(board, agent_coords.x, agent_coords.y - 1, width, _AGENT);

    if( stored_value != '0' )
        set_value( board, agent_coords.x, agent_coords.y, width, stored_value );
    else
        set_value( board, agent_coords.x, agent_coords.y, width, _EMPTY );

    return new_stored_value;
    
}

enum board_location move_down( enum board_location* board, int width, int height, enum board_location stored_value ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.y == height - 1 )
        return '0';

    enum board_location new_stored_value = get_value(board, agent_coords.x, agent_coords.y + 1, width);

    set_value(board, agent_coords.x, agent_coords.y + 1, width, _AGENT);

    if( stored_value != '0' )
        set_value( board, agent_coords.x, agent_coords.y, width, stored_value );
    else
        set_value( board, agent_coords.x, agent_coords.y, width, _EMPTY );

    return new_stored_value;
    
}

enum board_location move_right( enum board_location* board, int width, int height, enum board_location stored_value ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.x == width - 1 )
        return '0';

    enum board_location new_stored_value = get_value(board, agent_coords.x + 1, agent_coords.y, width);

    set_value(board, agent_coords.x + 1, agent_coords.y, width, _AGENT);

    if( stored_value != '0' )
        set_value( board, agent_coords.x, agent_coords.y, width, stored_value );
    else
        set_value( board, agent_coords.x, agent_coords.y, width, _EMPTY );

    return new_stored_value;
    
}

enum board_location move_left( enum board_location* board, int width, int height, enum board_location stored_value ){

    int agent_position = get_agent_position( board, width, height );
    struct coords agent_coords = get_coordinates(agent_position, width);

    if( agent_coords.x == 0 )
        return '0';

    enum board_location new_stored_char = get_value(board, agent_coords.x - 1, agent_coords.y, width);

    set_value(board, agent_coords.x - 1, agent_coords.y, width, _AGENT);

    if( stored_value != '0' )
        set_value( board, agent_coords.x, agent_coords.y, width, stored_value );
    else
        set_value( board, agent_coords.x, agent_coords.y, width, _EMPTY );

    return new_stored_char;
    
}



int get_agent_position( enum board_location* board, int width, int height ){

    int agent_position = 0;
    bool agent_found = false;

    int x = 0;
    int y = 0;

    while( agent_found == false ){

        if( get_value(board, x, y, width) == _AGENT){

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
