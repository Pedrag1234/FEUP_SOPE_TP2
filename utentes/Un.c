#include "Un.h"

int launchThreads(User * user)
{
    clock_t start_time;
    double time_elapsed = 0.0;
    double time_delta_next_thread;
    double time_last_thread = 0.0;
    start_time = clock()/1000;//ms

    while(time_elapsed < user->nsecs*1000)//ms
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

int main(int argc, char const *argv[])
{
    User * user = createUser();
    if (fillUser(user, argc, argv) != 0 || strlen(user->fifoname) == 0 || user->nsecs == 0)
    {
        printUsageClient();
    }
    else
    {
        printUser(user);
    }

    launchThreads(user);
   
    destroyUser(user);
    return 0;
    
}
