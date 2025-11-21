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


struct replay_data_t* initialize_replay_data( int num_states, float* state_inputs, int action, float  reward );

// Returns length of linked list after appending to end
int append_replay_data( struct replay_data_t* head, struct replay_data_t* latest_replay_data );

struct replay_data_t* sample_replay_data( struct replay_data_t* head, int index );

int delete_replay_data( struct replay_data_t* replay_data );

int delete_replay_ll( struct replay_data_t* head );

void policy_to_target( const struct network_t *policy_network, struct network_t** target_network );

float dqn_loss( struct network_t *policy_network, struct network_t* target_network );



#endif
