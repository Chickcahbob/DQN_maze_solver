#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

struct network_values_t{
   float *nodes;
   float *biases;
   float *weights;
};

enum activation_function{
    LINEAR,
    SIGMOID
};

struct network_args_t{
    int num_layers;
    int *nodes_per_layer;
    enum activation_function* functions;
};

struct network_t{
    struct network_values_t *network_values;
    struct network_args_t *network_args;
};

struct num_values_t{
    int num_nodes_and_biases;
    int num_weights;
};

//Takes user definied network arguments to generate initial network values
struct network_values_t* network_init( struct network_args_t* network_args );

//Gets the numver of weights, nodes, and biases in the network
struct num_values_t get_num_values( struct network_args_t* network_args );

//Assigns random values to weights, nodes, and biases in the range [0,max]
void initialize_random_values( const struct network_values_t* network_values, const struct num_values_t num_values, float max);

//Subroutine to calculate forward propogation of node values
void *forward_prop( struct network_t *network );

//Print weights, nodes, and biases to specified output stream
void fprint_network(FILE *__restrict stream, const struct network_values_t* network_values, const struct num_values_t num_values);

//Frees memory allocated for weights, nodes, and biases
void delete_network_values( struct network_values_t* network_values );

//Frees memory allocated for network arguments
void delete_network_args( struct network_args_t* network_network_args );

//Frees memory allocated for network
void delete_network( struct network_t *network );

#endif
