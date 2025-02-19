#include "neural_network.h"

struct network_t* network_init( struct network_arguments_t* args ){

    struct network_t * network = (struct network_t *) malloc( sizeof( struct network_t ) );

    struct num_values_t num_values = get_num_values( args );

    network->nodes = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases );
    network->biases = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases);
    network->weights = (float *) malloc( sizeof(float) * num_values.num_weights);


    return network;

}

struct num_values_t get_num_values( struct network_arguments_t* args ){

    struct num_values_t num_values;
    num_values.num_nodes_and_biases = 0;
    num_values.num_weights = 1;
    
    for( int i = 0; i < args->num_layers; i++ ){
        num_values.num_nodes_and_biases += args->nodes_per_layer[i];
        num_values.num_weights *= args->nodes_per_layer[i];
    }

    return num_values;

}

void free_network( struct network_t* network ){

    free(network->nodes);
    free(network->weights);
    free(network->biases);

    free( network );

    fprintf( stdout, "Call me Abraham Lincoln the way I be freein memory!\n" );

}

void free_network_arguments(struct network_arguments_t *network_args){

    free( network_args->nodes_per_layer );
    free( network_args );

}
