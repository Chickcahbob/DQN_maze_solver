#ifndef _MULTITHREADING_H
#define _MULTITHREADING_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct min_max{

    int min;
    int max;

};

long get_num_procs();

int addition( struct min_max* range);

int addition_multithreaded( int max_num );

#endif
