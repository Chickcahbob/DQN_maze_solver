#include "keyboard.h"
#include "board.h"

char get_direction(){

    char direction = '0';
    char input[10];
    size_t input_len;
    int state = 1;

        fgets( input, sizeof( input ), stdin );
        flush_input(input);

        input_len = strlen( input );

    for( int i = 0; i < (int)input_len; i++ ){

        if( input[i] == '\33' ){

            direction = input[i + 2];

        }

    }

    return direction;

}

void flush_input( char user_input[] ){

    if( strchr( user_input, '\n') == NULL ){

        int character;

        while( ( character = getchar()) != '\n' && character != EOF );

    } else {

        user_input[ strcspn( user_input, "\r\n" )] = 0;

    }



}
