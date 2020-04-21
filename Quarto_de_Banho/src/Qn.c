#include "Qn.h"

int main(int argc, char const *argv[])
{
    BathroomParser *Bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, Bp) != 0) || strlen(Bp->fifoname) == 0 || Bp->secs_f == 0)
    {
        printUsage();
    }
    else
    {
        printBathroomParser(Bp);
        int i = 0;
        time_t start, end;
        double elapsed;

        time(&start);
        time(&end);

        elapsed = difftime(end, start);
        while (elapsed <= Bp->nsecs)
        {
            time(&end);
            elapsed = difftime(end, start);
            //TODO: PROCESS REQUESTS AND SEND ANSWER
        }
    }

    destroyBathroomParser(Bp);
    return 0;
}

void printUsage()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}
