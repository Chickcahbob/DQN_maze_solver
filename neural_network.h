#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <sched.h>

enum activation_function{
    _LINEAR,
    _SIGMOID,
    _RELU
};

enum cost_function{

    _MEAN_SQUARED_ERROR

};

struct layer_index_range{
    int min;
    int max;
};

struct network_args_t{
    int num_layers;
    int *nodes_per_layer;
    enum activation_function* functions;
};

struct num_values_t{
    int num_nodes_and_biases;
    int num_weights;
};

struct network_values_t{
   float *nodes;
   float *biases;
   float *weights;
};

struct network_t{
    struct network_values_t *network_values;
    struct network_args_t *network_args;
    struct num_values_t *num_values;
};

struct forward_prop_thread_t{

    struct network_t *network;
    int min_max[2];
    int current_layer;

};

struct back_prop_thread_t{

    int thread_num;
    float* node_targets;
    int min_max[2];
    int current_layer;
    float learning_rate;
    enum cost_function cost;
    struct network_t *network;

};

struct back_prop_return_t{
    int thread_num;
    float* prev_node_deltas;
    float* bias_deltas;
};

struct targets_t{
    int num_targets;
    float *target_values;
};

//Takes user definied network arguments to generate initial network values
void network_init( struct network_t* network );

//Initializes random values for network output targets
void targets_init( struct targets_t* targets );

//Gets the numver of weights, nodes, and biases in the network
void get_num_values( struct network_t* network );

//Assigns random values to weights, nodes, and biases in the range [0,max]
void initialize_random_values( const struct network_values_t* network_values, const struct num_values_t *num_values, float max);

//Calculate forward propogation of node values
void forward_prop( struct network_t *network );

//Subroutine for forward propagation
void *thread_foward_prop( void *args );

// Subroutine for backwords propagation
void *thread_back_prop( void *args );

//Returns the action with the greatest float value
int select_action( struct network_t* network);

//Print weights, nodes, and biases to specified output stream
void fprint_network(FILE *__restrict stream, const struct network_values_t* network_values, const struct num_values_t *num_values);

//Frees memory allocated for weights, nodes, and biases
void delete_network_values( struct network_values_t* network_values );

//Frees memory allocated for network arguments
void delete_network_args( struct network_args_t* network_network_args );

//Frees memory allocated for network
void delete_network( struct network_t *network );

void delete_targets( struct targets_t *targets );

//Returns the starting index of the last layer
struct layer_index_range last_layer_start_index( struct network_args_t* network_args );

//Compares prediction values to target values
int error_calculation( struct network_t* network, struct targets_t* targets );

int load_inputs( float* inputs, struct network_t* network);

#endif
