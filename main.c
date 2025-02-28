#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"
#include "neural_network.h"
#include "dqn.h"

int play( int height, int width );

int main(){

    srand(time(NULL));

    const int height = 3;
    const int width = 3;

    enum board_location *board = create_board(width, height);

    print_board(board, width, height);

    /*
    int completed = play( height, width );

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

    nodes_per_layer_alias[0] = width * height;
    nodes_per_layer_alias[1] = 2;
    nodes_per_layer_alias[2] = 4;
    nodes_per_layer_alias[3] = 1;
    nodes_per_layer_alias[4] = 3;

    network_init( network );

    for( int node = 0; node < nodes_per_layer_alias[0]; node++ ){

        switch( board[node] ){

            case _AGENT:
                network->network_values->nodes[node] = 0.25;
                break;
            case _OBJECTIVE:
                network->network_values->nodes[node] = 1;
                break;
            case _HOLE:
                network->network_values->nodes[node] = -1;
                break;
            default:
                network->network_values->nodes[node] = 0;
                break;

        }

    }

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

    struct network_t *target_network = NULL;

    policy_to_target(network, &target_network);

    if( target_network == NULL )
        fprintf( stdout, "TARGET IS NULL!\n" );

    fprint_network( stdout, target_network->network_values, target_network->num_values);

    if( network != NULL )
        delete_network( network );

    if( target_network != NULL )
        delete_network(target_network);

    if( board )
        delete_board(board);

    return 0;

}

int play( int height, int width ){

    enum board_location stored_value = _EMPTY;
    enum board_location* board = create_board( width, height );
    int completed = -1;

    while( completed == -1 ){

        system("clear");
        print_board( board, width, height );
        fprintf( stdout, "Location of I: %d\n", get_agent_position( board, width, height ) );

        stored_value = move(board, width, height, stored_value);

        if( stored_value == _HOLE ){
            completed = 0;
        } else if ( stored_value == _OBJECTIVE ){
            completed = 1;
        }

    }

    system( "clear" );
    print_board( board, width, height );

    if( board != NULL )
        delete_board( board );

    return completed;

}
