#include "multithreading.h"

long get_num_procs(){

    long number_of_processors = sysconf( _SC_NPROCESSORS_ONLN );

    return number_of_processors;

}

int addition(struct min_max * range){

    int sum = 0;

    for( int i = range->min; i < range->max; i++ ){

        sum += i;

    }

    fprintf( stdout, "Sum = %d\n", sum );

    return 0;

}

int addition_multithreaded(int max_num){


    return 0;
}
