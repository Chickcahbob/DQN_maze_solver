#include "dqn.h"
#include "neural_network.h"

void policy_to_target(const struct network_t* policy_network, struct network_t** target_network){

    if( (*target_network) == NULL ){

        (*target_network) = (struct network_t *)malloc(sizeof(struct network_t) );

        (*target_network)->network_values = (struct network_values_t *)malloc( sizeof( struct network_values_t ) );
        (*target_network)->network_values->nodes = (float *)malloc(sizeof(float) * policy_network->num_values->num_nodes_and_biases );
        (*target_network)->network_values->biases = (float *)malloc(sizeof(float) * policy_network->num_values->num_nodes_and_biases );
        (*target_network)->network_values->weights = (float *)malloc(sizeof(float) * policy_network->num_values->num_weights );

        (*target_network)->num_values = (struct num_values_t *)malloc(sizeof( struct num_values_t ) );

        (*target_network)->network_args = (struct network_args_t *)malloc(sizeof( struct network_args_t) );
        (*target_network)->network_args->nodes_per_layer = (int *)malloc(sizeof(int) * policy_network->network_args->num_layers );
        (*target_network)->network_args->functions = (enum activation_function*)malloc( sizeof(enum activation_function) * policy_network->num_values->num_nodes_and_biases );
    }

    for( int weight = 0; weight < policy_network->num_values->num_weights; weight++ ){
        (*target_network)->network_values->weights[weight] = policy_network->network_values->weights[weight];
    }

    for( int node = 0; node < policy_network->num_values->num_nodes_and_biases; node++ ){

        (*target_network)->network_values->nodes[node] = policy_network->network_values->nodes[node];
        (*target_network)->network_values->biases[node] = policy_network->network_values->biases[node];
        
        (*target_network)->network_args->functions[node] = policy_network->network_args->functions[node];

    }

    for( int layer = 0; layer < policy_network->network_args->num_layers; layer++ ){

        (*target_network)->network_args->nodes_per_layer[layer] = policy_network->network_args->nodes_per_layer[layer];

    }

    (*target_network)->network_args->num_layers = policy_network->network_args->num_layers;
    (*target_network)->num_values->num_weights = policy_network->num_values->num_weights;
    (*target_network)->num_values->num_nodes_and_biases = policy_network->num_values->num_nodes_and_biases;

}

struct replay_data_t* initialize_replay_data( int num_states, float* state_inputs, int action, float  reward ){

    struct replay_data_t* tmp_replay_data = (struct replay_data_t*) malloc( sizeof( struct replay_data_t ) );

    tmp_replay_data->num_state_values = num_states;
    tmp_replay_data->state_values = (float*) malloc( sizeof( float ) * num_states );

    tmp_replay_data->reward = reward;
    tmp_replay_data->action = action;
    tmp_replay_data->next = NULL;

    return tmp_replay_data;

}

int append_replay_data( struct replay_data_t* head, struct replay_data_t* latest_replay_data ){

    if( head == NULL ){
        fprintf( stdout, "ERROR: Attempting to set head of replay data linked list to NULL in append_replay_data(). Use initialize_replay_data() to allocate memory.\n" );
        return -1;
    }

    if( latest_replay_data == NULL ){
        fprintf( stdout, "ERROR: Attempting to insert NULL value into replay data linked list. Use initialize_replay_data() to allocate memory.\n" );
        return -1;
    }

    struct replay_data_t* iterator = head;

    int index = 0;

    while( iterator->next != NULL ){

        iterator = iterator->next;
        index++;

    }

    iterator->next = latest_replay_data;

    return index;
    
}

struct replay_data_t* sample_replay_data( struct replay_data_t* head, int index ){

    struct replay_data_t* iterator = head;

    int i = 0;

    while( i < index ){
        if( iterator->next == NULL ){
            fprintf( stdout, "ERROR: Provided index: %d is greater than linked list size: %d.\n", index, i );
           return NULL; 
        } else {
            iterator = iterator->next;
        }
        i++;
    }

    assert( iterator != NULL );

    return iterator;

}

int delete_replay_data(struct replay_data_t *replay_data){

    if( replay_data == NULL )
        return 1;

    if( replay_data->state_values != NULL){
        free( replay_data->state_values );
        replay_data->state_values = NULL;
    }

    free( replay_data );
    replay_data = NULL;

    return 0;

}

int delete_replay_ll( struct replay_data_t* head ){

    if( head == NULL ){
        fprintf( stdout, "ERROR: Attempting to set head of replay data linked list to NULL in delete_replay_ll(). Use initialize_replay_data() to allocate memory.\n" );
        return -1;
    }
 
    struct replay_data_t* iterator = head;
    struct replay_data_t* iterator_next = NULL;

    while( iterator->next != NULL ){
        iterator_next = iterator->next;
        delete_replay_data(iterator);
        iterator = iterator_next;
    }
    iterator_next = iterator->next;
    
    return 0;

}

float dqn_loss( struct network_t* policy_network, struct network_t* target_network){

    struct targets_t* targets = (struct targets_t*) malloc( sizeof( struct targets_t ) );
    forward_prop( target_network );

    struct layer_index_range target_indexs = last_layer_start_index( target_network->network_args );

    targets->num_targets = target_indexs.max - target_indexs.min;

    targets_init( targets );

    assert( targets->target_values != NULL );

    for( int i = 0; i < targets->num_targets; i++ ){

        targets->target_values[i] = target_network->network_values->nodes[i + target_indexs.min];
    }

    error_calculation( policy_network, targets );

    delete_targets( targets );

}
