#ifndef _DQN_H
#define _DQN_H

#include <stdlib.h>
#include <assert.h>
#include "neural_network.h"

struct replay_data_t{

    int num_state_values;

    float action;
    float reward;
    float *state_values;

    struct replay_data_t* next;

};

// Returns length of linked list after appending to end
int initialize_replay_data( struct replay_data_t* head, int num_state_inputs, float* state_inputs, int action, float reward );

struct replay_data_t* sample_replay_data( struct replay_data_t* head, int index );

int delete_replay_data( struct replay_data_t* replay_data );

void policy_to_target( const struct network_t *policy_network, struct network_t** target_network );

float dqn_loss( struct network_t *policy_network, struct network_t* target_network );



#endif
