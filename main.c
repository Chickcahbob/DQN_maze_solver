#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"
#include "neural_network.h"
#include "dqn.h"

int main(){

    srand(time(NULL));

    const int height = 3;
    const int width = 3;

    const int board_size = width * height;

    enum board_location *board = create_board(width, height);

    print_board(board, width, height);

    /*
    int completed = play( height, width );

    if( completed == 1 )
        fprintf( stdout, "You win!\n" );
    else
        fprintf( stdout, "You lose.\n" ); 
    */

    // START NETWORK INITIALIZATION
    struct network_t *network = (struct network_t*) malloc( sizeof(struct network_t) );

    network->network_args = (struct network_args_t*) malloc( sizeof( struct network_args_t ) );

    network->network_args->num_layers = 5;
    int num_layers_alias = network->network_args->num_layers;

    network->network_args->nodes_per_layer = (int *) malloc( sizeof( int ) * num_layers_alias );
    int *nodes_per_layer_alias = network->network_args->nodes_per_layer;

    nodes_per_layer_alias[0] = board_size;
    nodes_per_layer_alias[1] = 2;
    nodes_per_layer_alias[2] = 4;
    nodes_per_layer_alias[3] = 1;
    nodes_per_layer_alias[4] = 4; //should be ouptut layer?

    network_init( network );

    //Training loop start
    float * state_inputs = (float*) malloc( sizeof( float ) * board_size);

    for( int node = 0; node < board_size; node++ ){

        switch( board[node] ){

            case _AGENT:
                network->network_values->nodes[node] = 0.25;
                state_inputs[node] = 0.25;
                break;
            case _OBJECTIVE:
                network->network_values->nodes[node] = 1;
                state_inputs[node] = 1;
                break;
            case _HOLE:
                network->network_values->nodes[node] = -1;
                state_inputs[node] = -1;
                break;
            case _EMPTY:
                network->network_values->nodes[node] = 0;
                state_inputs[node] = 0;
                break;
            default:
                network->network_values->nodes[node] = 0;
                state_inputs[node] = 0;
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

    // END OF NETOWQRK INITIALIZATION


    // Sample forward propagation
    forward_prop(network);

    int nn_action = select_action( network );

    //Creating first replay data struct as null to initialize linked list
    fprint_network( stdout, network->network_values, network->num_values);

    float reward = 0.5;

    struct replay_data_t* head = initialize_replay_data(board_size, state_inputs, nn_action, reward);

    forward_prop(network);

    struct replay_data_t* next_replay = initialize_replay_data(board_size, state_inputs,  nn_action, reward);

    int index = append_replay_data(head, next_replay );

    struct replay_data_t* sample = sample_replay_data( head, index);

    fprintf( stdout, "Sample data reward = %f\n", sample->reward );

    struct network_t *target_network = NULL;

    policy_to_target(network, &target_network);

    assert( target_network != NULL );


    
    dqn_loss( network, target_network );

    struct layer_index_range test = last_layer_start_index(target_network->network_args);

    if( head != NULL )
        delete_replay_ll(head);

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

    delete_board( board );

    return completed;

}
