#include "Un.h"
#include <stdio.h>
#include <time.h>

int launchThreads(user *us)
{
    clock_t start_time;
    double time_elapsed = 0.0;
    double time_delta_next_thread;
    double time_last_thread = 0.0;
    start_time = clock()/1000;//ms

    while(time_elapsed < us->nsecs*1000)//ms
    {
        time_elapsed = 1000*(((double) (clock() - start_time)) / CLOCKS_PER_SEC);//ms

        if((clock()/1000 - time_last_thread) > time_delta_next_thread)
        {
            srand(time(0));
            time_delta_next_thread = rand()%(MAX_TIME_BETWEEN_THREADS - MIN_TIME_BETWEEN_THREADS + 1) + MIN_TIME_BETWEEN_THREADS;//ms
            time_last_thread = clock()/1000;
            /* launch thread */
        }
    } 
    return 0;
}

void printUsage()
{
    printf("Usage: ./Un <-t nsecs> fifoname\n");
}

int main(int argc, char const *argv[])
{
    user *us = createUser();
    if (fillUser(us, argc, argv) != 0 || strlen(us->fifoname) == 0 || us->nsecs == 0)
    {
        printUsage();
    }
    else
    {
        printUser(us);
    }

    launchThreads(us);
   
    destroyUser(us);
    return 0;
    
}
