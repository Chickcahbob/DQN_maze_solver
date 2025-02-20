#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"
#include "neural_network.h"

int play( int height, int width );

int main(){

    srand(time(NULL));

    const int height = 10;
    const int width = 8;

    /*int completed = play( HEIGHT, WIDTH );

    if( completed == 1 )
        fprintf( stdout, "You win!\n" );
    else
        fprintf( stdout, "You lose.\n" ); 

    */

    struct network_t *network = (struct network_t*) malloc( sizeof(struct network_t) );

    network->network_args = (struct network_args_t*) malloc( sizeof( struct network_args_t ) );

    network->network_args->num_layers = 3;

    network->network_args->nodes_per_layer = (int *) malloc( sizeof( int ) * network->network_args->num_layers );

    network->network_args->nodes_per_layer[0] = 3;
    network->network_args->nodes_per_layer[1] = 5;
    network->network_args->nodes_per_layer[2] = 3;

    network->network_values = network_init( network->network_args );

    if( network->network_values != NULL )
        delete_network_values(network->network_values);

    if( network->network_args != NULL )
        delete_network_args(network->network_args);

    if( network != NULL )
        free( network );

    return 0;

}

int play( int height, int width ){

    char stored_char = '0';
    char* board = create_board( width, height );
    int completed = -1;

    while( completed == -1 ){

        system("clear");
        print_board( board, width, height );
        fprintf( stdout, "Location of I: %d\n", get_agent_position( board, width, height ) );

        stored_char = move(board, width, height, stored_char);

        if( stored_char == 'H' ){
            completed = 0;
        } else if ( stored_char == 'O' ){
            completed = 1;
        }

    }

    system( "clear" );
    print_board( board, width, height );

    delete_board( board );

    return completed;

}
