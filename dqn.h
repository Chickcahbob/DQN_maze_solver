#ifndef _DQN_H
#define _DQN_H

#include <stdlib.h>
#include <assert.h>
#include "neural_network.h"

struct q_table{
    int columns;
    int rows;
    float *values;
};

void policy_to_target( const struct network_t *policy_network, struct network_t** target_network );

void q_table_init( struct q_table* table );

void delete_q_table( struct q_table* table );

#endif
