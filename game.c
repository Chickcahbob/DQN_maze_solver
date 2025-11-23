#include "game.h"
#include "neural_network.h"

int play( int height, int width, bool ai ){

    enum board_location stored_value = _EMPTY;
    enum board_location* board = create_board( width, height );
    int completed = -1;

    if( ai ){

    }

    while( completed == -1 ){

        system("clear");
        print_board( board, width, height );
        fprintf( stdout, "Location of I: %d\n", get_agent_position( board, width, height ) );

        char direction;
        if( ai ){

        } else {
            direction = get_direction();
        }

        stored_value = move(board, width, height, stored_value, direction);

        if( stored_value == _HOLE ){
            completed = 0;
        } else if ( stored_value == _OBJECTIVE ){
            completed = 1;
        }

    }

    system( "clear" );
    print_board( board, width, height );

    delete_board( board );

    return completed;

}
