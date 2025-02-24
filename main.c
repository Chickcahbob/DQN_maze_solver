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

    network->network_args->num_layers = 5;
    int num_layers_alias = network->network_args->num_layers;

    network->network_args->nodes_per_layer = (int *) malloc( sizeof( int ) * num_layers_alias );
    int *nodes_per_layer_alias = network->network_args->nodes_per_layer;

    nodes_per_layer_alias[0] = 3;
    nodes_per_layer_alias[1] = 2;
    nodes_per_layer_alias[2] = 4;
    nodes_per_layer_alias[3] = 1;
    nodes_per_layer_alias[4] = 3;

    network_init( network );

    network->network_args->functions = (enum activation_function*)malloc(sizeof(enum activation_function) * network->num_values->num_nodes_and_biases);
    enum activation_function* functions_alias = network->network_args->functions;

    int layer_min = 0;
    int layer_max;

    for( int layer = 0; layer < num_layers_alias; layer++ ){

        layer_max = layer_min + nodes_per_layer_alias[layer];

        for( int function = layer_min; function < layer_max; function++ ){

            if( layer != num_layers_alias - 1 )
                functions_alias[function] = _LINEAR;
            else
                functions_alias[function] = _SIGMOID;

        }

        layer_min = layer_max;

    }

    forward_prop(network);

    fprint_network( stdout, network->network_values, network->num_values);

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
