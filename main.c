#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "board.h"
#include "movement.h"
#include "keyboard.h"

#define HEIGHT 10
#define WIDTH 9

int play( int height, int width );

int main(){

    srand(time(NULL));

    int completed = play( HEIGHT, WIDTH );

    if( completed == 1 )
        fprintf( stdout, "You win!\n" );
    else
        fprintf( stdout, "You lose.\n" ); 

    return 0;

}

int play( int height, int width ){

    char stored_char = '0';
    
    char* board = create_board( width, height );

    int completed = -1;

    while( completed == -1 ){

        system("clear");

        print_board( board, width, height );

        //int test_num = num_chars_near(board, width, height, 15, '_');

        //fprintf( stdout, "%d\n", test_num );

        //print_board( board, width, height );
        fprintf( stdout, "Location of I: %d\n", get_agent_position( board, width, height ) );

        stored_char = move(board, width, height, stored_char);

        if( stored_char == 'H' ){
            completed = 0;
        } else if ( stored_char == 'O' ){

            completed = 1;

        }

    }

    system( "clear" );
    print_board( board, width, height );

    delete_board( board );

    return completed;

}
