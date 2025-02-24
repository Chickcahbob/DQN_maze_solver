#include "neural_network.h"
#include <stdio.h>

void network_init( struct network_t* network ){

    network->network_values = (struct network_values_t *) malloc( sizeof( struct network_values_t ) );
    network->num_values = (struct num_values_t *) malloc( sizeof( struct num_values_t ) );

    get_num_values( network );

    network->network_values->nodes = (float *) malloc( sizeof(float) * network->num_values->num_nodes_and_biases );
    network->network_values->biases = (float *) malloc( sizeof(float) * network->num_values->num_nodes_and_biases);
    network->network_values->weights = (float *) malloc( sizeof(float) * network->num_values->num_weights);


    initialize_random_values(network->network_values, network->num_values, 0.2);

    //fprint_network( stdout, network->network_values, network->num_values);

}

void get_num_values( struct network_t* network ){

    network->num_values = (struct num_values_t*)malloc( sizeof( struct num_values_t) );
    struct num_values_t *num_values_alias = network->num_values;
    struct network_args_t *network_args_alias = network->network_args;
    network->num_values->num_nodes_and_biases = 0;
    network->num_values->num_weights = 1;
    
    for( int i = 0; i < network_args_alias->num_layers; i++ ){
        num_values_alias->num_nodes_and_biases += network_args_alias->nodes_per_layer[i];
        num_values_alias->num_weights *= network_args_alias->nodes_per_layer[i];
    }

}

void initialize_random_values(const struct network_values_t* network_values, const struct num_values_t *num_values, float max){

    for( int i = 0; i < num_values->num_nodes_and_biases; i++ ){
        network_values->nodes[i] = (float)rand()/(float)(RAND_MAX/max);
        network_values->biases[i] = (float)rand()/(float)(RAND_MAX/max);
    }

    for( int i = 0; i < num_values->num_weights; i++ ){
        network_values->weights[i] = (float)rand()/(float)(RAND_MAX/max);
    }

}

void *thread_forward_prop( void *args ){

    struct multithreading_nodes_t *thread_data = (struct multithreading_nodes_t *)args;
    struct network_values_t *values_alias = thread_data->network->network_values;
    struct network_args_t *args_alias = thread_data->network->network_args;

    int prev_layer_base = 0;
    int prev_layer_nodes = args_alias->nodes_per_layer[thread_data->current_layer - 1];
    int weight_min = 1;
    int weight_max;

    for( int layer = 0; layer < thread_data->current_layer - 1; layer++ ){
        prev_layer_base += args_alias->nodes_per_layer[layer];
        weight_min *= args_alias->nodes_per_layer[layer];
    }

    weight_min -= 1;

    for( int calc_node = thread_data->min_max[0]; calc_node < thread_data->min_max[1]; calc_node++ ){

        values_alias->nodes[calc_node] = values_alias->biases[calc_node];
        weight_max = weight_min + prev_layer_nodes;

        for( int prev_layer_node = prev_layer_base; prev_layer_node < prev_layer_base + prev_layer_nodes; prev_layer_node++ ){

            for( int weight = weight_min; weight < weight_max; weight++ ){
                values_alias->nodes[calc_node] += values_alias->nodes[prev_layer_node] * values_alias->weights[weight];
            }

        }

        if( values_alias->nodes[calc_node] > 1.0 )
            values_alias->nodes[calc_node] = 1.0;

        switch( args_alias->functions[calc_node] ){
            case _SIGMOID:
                values_alias->nodes[calc_node] = 1 / ( 1 + expf(-4 * values_alias->nodes[calc_node]));
                break;
            case _RELU:
                if( values_alias->nodes[calc_node] < 0 )
                    values_alias->nodes[calc_node] = 0;
            case _LINEAR:
                break;
        }

        weight_min = weight_max;
    }

    return NULL;

}

void forward_prop( struct network_t* network){

    int num_cores = (int)sysconf(_SC_NPROCESSORS_ONLN);

    int num_layers_alias = network->network_args->num_layers;
    int *nodes_per_layer_alias = network->network_args->nodes_per_layer;
    
    int *calcs_per_core;

    int max_threads;

    struct multithreading_nodes_t * multithreading_nodes = calloc(num_cores, sizeof(struct multithreading_nodes_t));
    pthread_t threads[num_cores];

    int layer_base = network->network_args->nodes_per_layer[0];

    for( int cur_layer = 1; cur_layer < num_layers_alias; cur_layer++ ){

        calcs_per_core = calloc( num_cores, sizeof(int) );

        for( int node_to_calc = 0; node_to_calc < nodes_per_layer_alias[cur_layer]; node_to_calc++ )
            calcs_per_core[node_to_calc % num_cores]++;
        
        if( num_cores < nodes_per_layer_alias[cur_layer] )
            max_threads = num_cores;
        else
            max_threads = nodes_per_layer_alias[cur_layer];

        for( int thread_num = 0; thread_num < max_threads; thread_num++){

            multithreading_nodes[thread_num].network = network;
            multithreading_nodes[thread_num].min_max[0] = layer_base;
            multithreading_nodes[thread_num].min_max[1] = layer_base + calcs_per_core[thread_num];
            multithreading_nodes[thread_num].current_layer = cur_layer;


           pthread_create(&threads[thread_num], NULL, thread_forward_prop, (void *) &multithreading_nodes[thread_num]);

           layer_base += calcs_per_core[thread_num];
        }

        for( int thread_num = 0; thread_num < max_threads; thread_num++ ){
            pthread_join( threads[thread_num], NULL );
        }

        free( calcs_per_core );

    }

    free( multithreading_nodes );

            
}

//Print current data stored in the network to specified output stream type
void fprint_network( FILE *__restrict stream, const struct network_values_t* network_values, const struct num_values_t *num_values){

    fprintf( stream, "WEIGHTS: " );

    for( int i = 0; i < num_values->num_weights; i++ ){

        fprintf( stream, "%lf", network_values->weights[i] );

        if( i != num_values->num_weights - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }

    fprintf( stream, "BIASES: " );

    for( int i = 0; i < num_values->num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network_values->biases[i] );

        if( i != num_values->num_nodes_and_biases - 1 )
            fprintf( stream, ", " );
        else
            fprintf( stream, "\n\n" );

    }
    
    fprintf( stream, "CURRENT NODE OUTPUT: " );

    for( int i = 0; i < num_values->num_nodes_and_biases; i++ ){

        fprintf( stream, "%lf", network_values->nodes[i] );

        if( i != num_values->num_nodes_and_biases - 1 )
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

void delete_network_args(struct network_args_t *network_args){

    if( network_args->nodes_per_layer != NULL )
        free( network_args->nodes_per_layer );

    if( network_args->functions != NULL )
        free( network_args->functions );

    if( network_args != NULL )
        free( network_args );

}

void delete_network( struct network_t *network ){

    if( network->num_values != NULL )
        free( network->num_values );

    if( network->network_args != NULL )
        delete_network_args( network->network_args );

    if( network->network_values != NULL )
        delete_network_values( network->network_values);

    if( network != NULL )
        free( network );

}
