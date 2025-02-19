#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"
#include "neural_network.h"

#define HEIGHT 10
#define WIDTH 9

int play( int height, int width );

int main(){

    srand(time(NULL));

    /*int completed = play( HEIGHT, WIDTH );

    if( completed == 1 )
        fprintf( stdout, "You win!\n" );
    else
        fprintf( stdout, "You lose.\n" ); 

    */

    struct network_arguments_t* network_arguments = (struct network_arguments_t*) malloc( sizeof( struct network_arguments_t ) );

    network_arguments->num_layers = 3;

    network_arguments->nodes_per_layer = (int *) malloc( sizeof( int ) * network_arguments->num_layers );

    network_arguments->nodes_per_layer[0] = 3;
    network_arguments->nodes_per_layer[1] = 5;
    network_arguments->nodes_per_layer[2] = 3;

    struct network_t* network = network_init( network_arguments );

    if( network != NULL )
        free_network(network);

    if( network_arguments != NULL )
        free_network_arguments(network_arguments);

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
