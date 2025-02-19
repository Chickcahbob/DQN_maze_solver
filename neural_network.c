#include "neural_network.h"
#include <stdlib.h>

struct network_t* network_init( struct network_arguments_t* args ){

    struct network_t * network = (struct network_t *) malloc( sizeof( struct network_t ) );

    struct num_values_t num_values = get_num_values( args );

    network->nodes = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases );
    network->biases = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases);
    network->weights = (float *) malloc( sizeof(float) * num_values.num_weights);

    initialize_values(network, num_values);

    fprint_network( stdout, network, num_values);

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

void initialize_values(struct network_t* network, struct num_values_t num_values){

    float max_value = 0.3;

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){
        network->nodes[i] = (float)rand()/(float)(RAND_MAX/max_value);
        network->biases[i] = (float)rand()/(float)(RAND_MAX/max_value);
    }

    for( int i = 0; i < num_values.num_weights; i++ ){
        network->weights[i] = (float)rand()/(float)(RAND_MAX/max_value);
    }

}

void fprint_network( FILE *__restrict stream, struct network_t* network, struct num_values_t num_values){

    fprintf( stream, "WEIGHTS: " );

    for( int i = 0; i < num_values.num_weights; i++ ){

        fprintf( stream, "%lf", network->weights[i] );

        if( i != num_values.num_weights - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }

    fprintf( stream, "BIASES " );

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network->biases[i] );

        if( i != num_values.num_nodes_and_biases - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }
    
    fprintf( stream, "CURRENT NODE OUTPUT: " );

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network->nodes[i] );

        if( i != num_values.num_nodes_and_biases - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n" );

    }



}

void forward_prop(struct network_t *network, struct network_arguments_t *args){

}

void delete_network( struct network_t* network ){

    free(network->nodes);
    free(network->weights);
    free(network->biases);

    free( network );

}

void delete_network_arguments(struct network_arguments_t *network_args){

    free( network_args->nodes_per_layer );
    free( network_args );

}
