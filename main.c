#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"
#include "neural_network.h"
#include "dqn.h"
#include "game.h"

int main(){

    srand(time(NULL));

    const int height = 3;
    const int width = 3;

    bool ai = true;

    /*
    int completed = play( height, width );

    if( completed == 1 )
        fprintf( stdout, "You win!\n" );
    else
        fprintf( stdout, "You lose.\n" ); 
    */

    int completed = play(height, width, ai);





/*    struct network_t *target_network = NULL;

    policy_to_target(network, &target_network);

    assert( target_network != NULL );


    
    dqn_loss( network, target_network );
*/

 //   struct layer_index_range test = last_layer_start_index(target_network->network_args);

      return 0;

}
