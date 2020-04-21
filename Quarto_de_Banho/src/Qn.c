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
        //time_t start, end;
        //double elapsed;

        char fifoname[261];
        strcpy(fifoname, "");

        sprintf(fifoname, "/tmp/%s", Bp->fifoname);
        mkfifo(fifoname, 0666);
        srand(time(0));

        request_list *r = createList();

        for (int i = 1; i <= (rand() % 1024); i++)
        {
            char aux[256];
            strcpy(aux, "");
            sprintf(aux, "%d", i);
            addRequest(aux, r);
        }

        for (int i = 0; i < r->size; i++)
        {
            printf("I = %d || String = %s\n", i, r->requests[i]);
        }
        printf("================================================================\n");

        for (int i = 0; i < (rand() % r->size); i++)
        {
            requestProcessed(r);
        }

        for (int i = 0; i < r->size; i++)
        {
            printf("I = %d || String = %s\n", i, r->requests[i]);
        }

        destroyList(r);
        /*time(&start);
        time(&end);

        elapsed = difftime(end, start);
        while (elapsed <= Bp->nsecs)
        {
            time(&end);
            elapsed = difftime(end, start);
            //TODO: PROCESS REQUESTS AND SEND ANSWER
        }*/
    }

    destroyBathroomParser(Bp);
    return 0;
}

void printUsage()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}
