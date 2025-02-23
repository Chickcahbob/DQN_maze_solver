#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
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
    int num_layers_alias = network->network_args->num_layers;

    network->network_args->nodes_per_layer = (int *) malloc( sizeof( int ) * num_layers_alias );
    int *nodes_per_layer_alias = network->network_args->nodes_per_layer;

    nodes_per_layer_alias[0] = 5;
    nodes_per_layer_alias[1] = 26;
    nodes_per_layer_alias[2] = 1;

    network_init( network );

    network->network_args->functions = (enum activation_function*)malloc(sizeof(enum activation_function) * network->num_values->num_nodes_and_biases);
    enum activation_function* functions_alias = network->network_args->functions;

    for( int i = 0; i < num_layers_alias; i++ ){

        if( i == num_layers_alias - 1 )
            functions_alias[i] = _LINEAR;
        else
            functions_alias[i] = _SIGMOID;
    }


    forward_prop(network);

    if( network != NULL )
        delete_network( network );

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

    if( board != NULL )
        delete_board( board );

    return completed;

}
