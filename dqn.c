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

    free( targets );

}
