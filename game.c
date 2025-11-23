#include "game.h"
#include "board.h"
#include "dqn.h"
#include "neural_network.h"

int play( int height, int width, bool ai ){

    int completed = -1;

    if( ai ){

        completed = ai_play(width, height);
    } else {
        completed = user_play( width, height );
    }


    return completed;

}

int user_play( int width, int height ){

    enum board_location stored_value = _EMPTY;
    enum board_location* board = create_board( width, height );

    int completed = -1;

    while( completed == -1 ){

        system("clear");
        print_board( board, width, height );
        fprintf( stdout, "Location of I: %d\n", get_agent_position( board, width, height ) );

        char direction = get_direction();

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

int ai_play( int width, int height ){

    const int board_size = width * height;
    int completed = -1;

    // INITIALIZE POLICY NETWORK
    // TODO: Put this into a function on it's own
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

    // END INITIALIZE POLICY NETWORK

    // START TRAINING LOOP
    // TODO: Put this into it's own function
    int i = 0;

    enum board_location stored_value, next_location;
    enum board_location* board = NULL;
    struct replay_data_t* head;
    struct replay_data_t* next_replay;
    struct replay_data_t* sample;

    head = NULL;
    float reward;
    int replay_index = 0;
    int sample_index = 0;

    while( i == 0 ){

        stored_value = _EMPTY;
        board = create_board( width, height);
        assert( board != NULL );

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
        print_board(board, width, height);
        // START REPLAY CREATION
        forward_prop(network);
        int nn_action = select_action( network );

        next_location = selection_translate(  board, stored_value, height, width, nn_action );

        switch( next_location ){
            case _OBJECTIVE:
                reward = 1;
                break;
            case _HOLE:
                reward = -1;
                break;
            default:
                reward = 0;
                break;
 
        }

        if( head == NULL ){
            head = initialize_replay_data(board_size, state_inputs, nn_action, reward);
        } else {
            next_replay = initialize_replay_data(board_size, state_inputs, nn_action, reward);
            replay_index = append_replay_data(head, next_replay );

        }

        sample_index = rand() % ( replay_index + 1 );

        sample = sample_replay_data( head, sample_index);
        fprintf( stdout, "Sample data reward = %f\n", sample->reward );

        print_board(board, width, height);
        delete_board( board );
        i = 1;
    }
    // END TRAINING LOOP
    
    //STRART MEMORY CLEANING
      if( head != NULL )
        delete_replay_ll(head);

    if( network != NULL )
        delete_network( network );

    return completed;
}


enum board_location selection_translate( enum board_location* board, enum board_location stored_value, int height, int width, int action ){

    char direction;
    enum board_location next_location;

    assert( action >= 0 );
    assert( action < 4 );

    switch( action ){
        case 0:
            direction = 'A';
            break;
        case 1:
            direction = 'B';
            break;
        case 2:
            direction = 'C';
            break;
        default:
            direction = 'D';
            break;

    }

    next_location = move(board, width, height, stored_value, direction);

    return next_location;
    
}
