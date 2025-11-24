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
    float discount_factor = 0.01;

    struct network_t* target_network = NULL;

    enum board_location stored_value, next_location;
    enum board_location* board = NULL;
    enum board_location* prev_board = NULL;

    struct replay_data_t* head = NULL;
    struct replay_data_t* next_replay;
    struct replay_data_t* sample;

    float reward, target_value;
    int replay_index = 0;
    int sample_index = 0;

    float* state_inputs;
    float* next_state_inputs;

    int target_update_rounds = 2;

    int nn_action, tn_action;

    int i = 0;
    while( i < 3 ){

        fprintf( stdout, "########### TRAINING ROUND: %d ##########\n", i );
        if( i % target_update_rounds == 0 ){
            policy_to_target( network, &target_network );

            fprintf( stdout, "POLICY NETWORK UPDATED\n");
        }

        stored_value = _EMPTY;
        board = create_board( width, height);
        assert( board != NULL );

        state_inputs = convert_inputs( board, board_size, network );

        //print_board(board, width, height);

        // GENERATE NEW REPLAY DATA

        //fprintf( stdout, "GENERATING NEW REPLAY DATA\n" );
        forward_prop(network);
        nn_action = select_action( network );

        save_board(&prev_board, board, width, height);

        next_location = selection_translate(  board, stored_value, height, width, nn_action );
        //fprintf( stdout, "Movement\n" );
        //print_board(board, width, height);

        reward = convert_reward(next_location);

        next_state_inputs = convert_inputs(board, board_size, target_network);

        if( head == NULL ){
            head = initialize_replay_data(board_size, state_inputs, nn_action, reward, next_state_inputs);
        } else {
            next_replay = initialize_replay_data(board_size, state_inputs, nn_action, reward, next_state_inputs);
            replay_index = append_replay_data(head, next_replay );
        }

        // SELECT RANDOM REPLAY DATA

        sample_index = rand() % ( replay_index + 1 );

        fprintf( stdout, "REPLAYING %d OF %d\n", sample_index + 1, replay_index + 1 );

        sample = sample_replay_data( head, sample_index);
        fprintf( stdout, "Sample data reward = %f\n", sample->reward );
        reconstruct_board(board, sample->state_values, board_size);
        print_board(board, width, height);
        
        load_inputs(sample->next_state_values, target_network );

        forward_prop( target_network );

        tn_action = select_action( target_network );

        reconstruct_board(board, sample->next_state_values, board_size);
        stored_value = previous_move(sample->reward);

        fprintf( stdout, "TARGET NETWORK TAKING NEXT STEP\n" );
        next_location = selection_translate( board, stored_value, height, width, tn_action );

        target_value = bellman_equation(sample->reward, discount_factor, reward);

        print_board(board, width, height);

        fprintf( stdout, "Target value from Bellman Equation: %f\n", target_value );

        free( state_inputs );
        free( next_state_inputs );

        if( board != NULL )
            delete_board( board );

        if( prev_board != NULL )
            delete_board( prev_board );

        i++;
    }
    // END TRAINING LOOP
    
    //STRART MEMORY CLEANING
    if( head != NULL )
        delete_replay_ll(head);

    if( network != NULL )
        delete_network( network );
    
    if( target_network != NULL )
        delete_network( target_network );

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

float* convert_inputs( enum board_location* board, int board_size, struct network_t* network ){

    float* inputs = (float*) malloc( sizeof( float ) * board_size);

    for( int node = 0; node < board_size; node++ ){

        switch( board[node] ){

            case _AGENT:
                network->network_values->nodes[node] = 0.25;
                inputs[node] = 0.25;
                break;
            case _OBJECTIVE:
                network->network_values->nodes[node] = 1;
                inputs[node] = 1;
                break;
            case _HOLE:
                network->network_values->nodes[node] = -1;
                inputs[node] = -1;
                break;
            case _EMPTY:
                network->network_values->nodes[node] = 0;
                inputs[node] = 0;
                break;
            default:
                network->network_values->nodes[node] = 0;
                inputs[node] = 0;
                break;

        }

    }

    return inputs;

}

float convert_reward( enum board_location next_location ){
    float reward;

    switch( next_location ){
        case _OBJECTIVE:
            reward = 1;
            break;
        case _HOLE:
            reward = -1;
            break;
        case _AGENT:
            reward = -0.25;
            break;
 
        default:
            reward = 0;
            break;
    }

    return reward;

}

void reconstruct_board( enum board_location* board, float* state_inputs, int board_size ){

    for( int i = 0; i < board_size; i++ ){

        if( state_inputs[i] == -1 ){
            board[i] = _HOLE;
        } else if ( state_inputs[i] == 0.25 ){
            board[i] = _AGENT;
        } else if ( state_inputs[i] == 1 ){
            board[i] = _OBJECTIVE;
        } else {
            board[i] = _EMPTY;
        }
    }

}

enum board_location previous_move( float prev_reward ){

    enum board_location prev_move;

    if( prev_reward == -1 ){
        prev_move = _HOLE;
    } else if ( prev_reward == 0.25 ){
        prev_move = _AGENT;
    } else if ( prev_reward == 1 ){
        prev_move = _OBJECTIVE;
    } else {
        prev_move = _EMPTY;
    }


    return prev_move;
}
