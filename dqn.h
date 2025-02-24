#ifndef _DQN_H
#define _DQN_H

#include <stdlib.h>
#include "neural_network.h"

void policy_to_target( const struct network_t *policy_network, struct network_t** target_network );

#endif
