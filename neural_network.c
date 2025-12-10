#include "neural_network.h"
#include <cstdlib>
#include <stdlib.h>

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

void targets_init( struct targets_t* targets){
    targets->target_values = (float *) malloc( sizeof(float) * targets->num_targets );
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

    float abs_range = max * 2;

    for( int i = 0; i < num_values->num_nodes_and_biases; i++ ){
        network_values->nodes[i] = (float)rand()/(float)(RAND_MAX/abs_range) - max;
        network_values->biases[i] = (float)rand()/(float)(RAND_MAX/abs_range) - max;
    }

    for( int i = 0; i < num_values->num_weights; i++ ){
        network_values->weights[i] = (float)rand()/(float)(RAND_MAX/abs_range) - max;
    }

}

void *thread_forward_prop( void *args ){

    struct forward_prop_thread_t* thread_data = (struct forward_prop_thread_t *)args;
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

            values_alias->nodes[calc_node] += values_alias->nodes[prev_layer_node] * values_alias->weights[weight_min + prev_layer_node];

        }

        if( values_alias->nodes[calc_node] > 1.0 )
            values_alias->nodes[calc_node] = 1.0;

        switch( args_alias->functions[calc_node] ){
            case _SIGMOID:
                values_alias->nodes[calc_node] = 1 / ( 1 + expf(-1 * values_alias->nodes[calc_node]));
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

void *thread_back_prop( void *args ){

    struct back_prop_thread_t* thread_data = (struct back_prop_thread_t*)args;
    struct network_t* network = thread_data->network;
    struct network_values_t *values_alias = network->network_values;
    struct network_args_t *args_alias = network->network_args;

    int prev_layer_base = 0;
    int prev_layer_nodes =  args_alias->nodes_per_layer[thread_data->current_layer - 1];
    int weight_min = 1;
    int weight_max;
    int num_nodes = thread_data->min_max[1] - thread_data->min_max[0];

    struct back_prop_return_t* return_values = (struct back_prop_return_t*) malloc (sizeof( struct back_prop_return_t ) );

    // Return delta for biases calculated for the range of nodes looped over
    // Return deltas for all previous layer nodes
    return_values->bias_deltas = (float*) calloc(num_nodes, sizeof(float));
    return_values->prev_node_deltas = (float*) calloc(prev_layer_nodes, sizeof(float));


    for( int layer = 0; layer < thread_data->current_layer - 1; layer++ ){

        prev_layer_base += args_alias->nodes_per_layer[layer]; 
        weight_min *= args_alias->nodes_per_layer[layer]; 
    }

    weight_min -= 1;

    float delta_C;
    float delta;
    float weight_delta;
    float prev_node_delta;
    float bias_delta;
    float sigmoid;

    for( int calc_node = thread_data->min_max[0]; calc_node < thread_data->min_max[1]; calc_node++ ){

        values_alias->nodes[calc_node] = values_alias->biases[calc_node];
        weight_max = weight_min + prev_layer_nodes;

        switch( thread_data->cost ){

            default:
                // MEAN SQUARED ERROR ( aL - y ) ^ 2
                delta_C = (values_alias->nodes[calc_node] - thread_data->node_targets[calc_node] );
                delta_C = delta_C * delta_C;
                break;

        }

        prev_node_delta = 0;
        bias_delta = 0;
        for( int prev_layer_node = prev_layer_base; prev_layer_node < prev_layer_base + prev_layer_nodes; prev_layer_node++ ){

            delta = delta_C * thread_data->learning_rate;

            // dC/daL: Derivative of cost functions
            switch( thread_data->cost ){
                default:
                    // Derivative of MSE 2 * ( aL - y )
                    delta /= (2 * ( values_alias->nodes[calc_node] - thread_data->node_targets[calc_node] ));
                    break;
            }

            // daL/dZL
            switch( args_alias->functions[calc_node] ){

                // sigmoid function: o(x)
                // Derivative of sigmoid function: o(x) * (1 - o(x) )
                case _SIGMOID:
                sigmoid = 1 / ( 1 + expf(-1 * values_alias->nodes[calc_node]));
                delta /= ( sigmoid * ( 1 - sigmoid ));
                    break;
                default:
                    // If _LINEAR or _RELU, daL/dZL = 1 so nothing changes
                    break;

            }

            // Divide weight changes evenly among all layers
            delta /= thread_data->current_layer;

            // Divide changes evenly among each node in the previous laye
            delta /= prev_layer_nodes;


            // Bias Delta Here dZL/dB = 1
            return_values->bias_deltas[calc_node - thread_data->min_max[0]] += delta;

            // Previous node delta dZL/a(L-1)
            return_values->prev_node_deltas[prev_layer_node - prev_layer_base] += delta / (values_alias->weights[weight_min + prev_layer_node] * prev_layer_nodes) ;

            // dZL/dw: Derivative of weight with respect to ZL
            weight_delta = delta / values_alias->nodes[prev_layer_node];

            // Update weights
            values_alias->weights[weight_min + prev_layer_node] += weight_delta;

        }

        weight_min = weight_max;

    }   

    return_values->thread_num = thread_data->thread_num;

    return (void *)return_values;
}

void forward_prop( struct network_t* network){

    int num_cores = (int)sysconf(_SC_NPROCESSORS_ONLN);

    int num_layers_alias = network->network_args->num_layers;
    int *nodes_per_layer_alias = network->network_args->nodes_per_layer;
    
    int *calcs_per_core;

    int max_threads;

    struct forward_prop_thread_t * forward_prop_thread = calloc(num_cores, sizeof(struct forward_prop_thread_t));
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

            forward_prop_thread[thread_num].network = network;
            forward_prop_thread[thread_num].min_max[0] = layer_base;
            forward_prop_thread[thread_num].min_max[1] = layer_base + calcs_per_core[thread_num];
            forward_prop_thread[thread_num].current_layer = cur_layer;


           pthread_create(&threads[thread_num], NULL, thread_forward_prop, (void *) &forward_prop_thread[thread_num]);

           layer_base += calcs_per_core[thread_num];
        }

        for( int thread_num = 0; thread_num < max_threads; thread_num++ ){
            pthread_join( threads[thread_num], NULL );
        }

        free( calcs_per_core );

    }

    free( forward_prop_thread );

            
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

int select_action( struct network_t* network){

    struct layer_index_range last_layer;

    struct network_values_t* network_values_ptr = network->network_values;

    last_layer = last_layer_start_index( network->network_args );

    int layer_range = last_layer.max - last_layer.min;

    int action = 0;
    
    //Asserts the network has an output layer
    assert( layer_range != 0 );

    //If the output layer is one node then it selects the only action available
    if( layer_range == 1 )
        return action;

    float greatest_output = network_values_ptr->nodes[last_layer.min];

    for( int output_i = last_layer.min + 1; output_i < last_layer.max; output_i++ ){

        if( network_values_ptr->nodes[output_i] > greatest_output ){
            greatest_output = network_values_ptr->nodes[output_i];
            action = output_i - last_layer.min;
        }
    }

    return action;

}

void delete_network_values( struct network_values_t* network_values ){

    if( network_values->nodes != NULL ){
        free(network_values->nodes);
        network_values->nodes = NULL;
    }

    if( network_values->weights != NULL ){
        free(network_values->weights);
        network_values->weights = NULL;
    }

    if( network_values->biases != NULL ){
        free(network_values->biases);
        network_values->biases = NULL;
    }


    if( network_values != NULL ){
        free( network_values );
        network_values = NULL;
    }

}

void delete_network_args(struct network_args_t *network_args){

    if( network_args->nodes_per_layer != NULL ){
        free( network_args->nodes_per_layer );
        network_args->nodes_per_layer = NULL;
    }

    if( network_args->functions != NULL ){
        free( network_args->functions );
        network_args->functions = NULL;
    }

    if( network_args != NULL ){
        free( network_args );
        network_args = NULL;
    }

}

void delete_network( struct network_t *network ){

    if( network->num_values != NULL ){
        free( network->num_values );
        network->num_values = NULL;
    }

    if( network->network_args != NULL ){
        delete_network_args( network->network_args );
        network->network_args = NULL;
    }

    if( network->network_values != NULL ){
        delete_network_values( network->network_values);
        network->network_values = NULL;
    }

    if( network != NULL ){
        free( network );
        network = NULL;
    }

}

void delete_targets( struct targets_t *targets){
    if( targets->target_values != NULL ){
        free( targets->target_values );
        targets->target_values = NULL;
    }

    if( targets != NULL ){
        free( targets );
        targets = NULL;
    }

}

struct layer_index_range last_layer_start_index( struct network_args_t* network_args ){
    struct layer_index_range layer_range;
    layer_range.min = 0;

    int layer = 0;
    while( layer < (network_args->num_layers - 1 )){
        layer_range.min += network_args->nodes_per_layer[layer];
        layer++;
    }

    layer_range.max = layer_range.min + network_args->nodes_per_layer[layer];

    return layer_range;

}

int error_calculation( struct network_t* network, struct targets_t* targets ){

   struct layer_index_range layer_range = last_layer_start_index(network->network_args); 

   assert( targets->num_targets == (layer_range.max - layer_range.min) );

    float raw_error;
    float rms_error;
    for( int target_index = 0; target_index < targets->num_targets; target_index++ ){
        raw_error = targets->target_values[target_index] - network->network_values->nodes[target_index + layer_range.min];
        rms_error = sqrtf(raw_error * raw_error);
        fprintf( stdout, "Output %d Error: Raw-%f RMS-%f\n", target_index, raw_error, rms_error );
    }

   return 0;

}

int load_inputs( float* inputs, struct network_t* network){

    int success = 1;

    assert( network != NULL );
    assert( network->network_values != NULL );
    assert( network->network_values->nodes != NULL );

    for( int index = 0; index < network->network_args->nodes_per_layer[0]; index++ ){

        network->network_values->nodes[index] = inputs[index];

    }

    success = 0;

    return success;

}
