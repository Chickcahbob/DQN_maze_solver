#include "neural_network.h"

struct network_values_t* network_init( struct network_args_t* network_args ){

    struct network_values_t * network_values = (struct network_values_t *) malloc( sizeof( struct network_values_t ) );

    struct num_values_t num_values = get_num_values( network_args );

    network_values->nodes = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases );
    network_values->biases = (float *) malloc( sizeof(float) * num_values.num_nodes_and_biases);
    network_values->weights = (float *) malloc( sizeof(float) * num_values.num_weights);

    initialize_random_values(network_values, num_values, 0.4);

    fprint_network( stdout, network_values, num_values);

    return network_values;

}

struct num_values_t get_num_values( struct network_args_t* network_args ){

    struct num_values_t num_values;
    num_values.num_nodes_and_biases = 0;
    num_values.num_weights = 1;
    
    for( int i = 0; i < network_args->num_layers; i++ ){
        num_values.num_nodes_and_biases += network_args->nodes_per_layer[i];
        num_values.num_weights *= network_args->nodes_per_layer[i];
    }

    return num_values;

}

void initialize_random_values(const struct network_values_t* network_values, const struct num_values_t num_values, float max){

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){
        network_values->nodes[i] = (float)rand()/(float)(RAND_MAX/max);
        network_values->biases[i] = (float)rand()/(float)(RAND_MAX/max);
    }

    for( int i = 0; i < num_values.num_weights; i++ ){
        network_values->weights[i] = (float)rand()/(float)(RAND_MAX/max);
    }

}

void *_forward_prop(struct network_t *network){
    /*TODO: 1. Divide a layer of nodes amongst threads
            2. Pass associated nodes for thread to evaluate through void *args
            2. Summation of (prev_layer_nodes * linked_weights ) + associated bias
            3. Perform sigmoid activation function if specified
    */
    return NULL;

}

//Print current data stored in the network to specified output stream type
void fprint_network( FILE *__restrict stream, const struct network_values_t* network_values, const struct num_values_t num_values){

    fprintf( stream, "WEIGHTS: " );

    for( int i = 0; i < num_values.num_weights; i++ ){

        fprintf( stream, "%lf", network_values->weights[i] );

        if( i != num_values.num_weights - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }

    fprintf( stream, "BIASES: " );

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network_values->biases[i] );

        if( i != num_values.num_nodes_and_biases - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }
    
    fprintf( stream, "CURRENT NODE OUTPUT: " );

    for( int i = 0; i < num_values.num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network_values->nodes[i] );

        if( i != num_values.num_nodes_and_biases - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n" );

    }

}

void delete_network_values( struct network_values_t* network_values ){

    if( network_values->nodes != NULL )
        free(network_values->nodes);

    if( network_values->weights != NULL )
        free(network_values->weights);

    if( network_values->biases != NULL )
        free(network_values->biases);

    if( network_values != NULL )
        free( network_values );

}

void delete_network_args(struct network_args_t *network_network_args){

    if( network_network_args->nodes_per_layer != NULL )
        free( network_network_args->nodes_per_layer );
    if( network_network_args != NULL )
        free( network_network_args );

}

void delete_network( struct network_t *network ){

    if( network->network_args != NULL )
        delete_network_args( network->network_args );

    if( network->network_values != NULL )
        delete_network_values( network->network_values);

    if( network != NULL )
        free( network );

}
