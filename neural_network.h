#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct network_t{

   float *nodes;
   float *biases;
   float *weights;

};

enum activation_function{

    LINEAR,
    SIGMOID

};

struct network_arguments_t{

    int num_layers;
    int *nodes_per_layer;
    enum activation_function* functions;

};

struct num_values_t{

    int num_nodes_and_biases;
    int num_weights;

};

struct network_t* network_init( struct network_arguments_t* args );

struct num_values_t get_num_values( struct network_arguments_t* args );

void initialize_values( struct network_t* network, struct num_values_t num_values );

void fprint_network(FILE *__restrict stream, struct network_t* network, struct num_values_t num_values);

void free_network( struct network_t* network );

void free_network_arguments( struct network_arguments_t* network_args );

#endif
