#include "BathroomParser.h"

BathroomParser *createBathroomParser()
{
    BathroomParser *Bp = (BathroomParser *)malloc(sizeof(BathroomParser));

    Bp->secs_f = 0;
    Bp->nsecs = 0;
    Bp->place_f = 0;
    Bp->nplaces = 0;
    Bp->thread_f = 0;
    Bp->nthreads = 0;
    strcpy(Bp->fifoname, "");

    return Bp;
}

void destroyBathroomParser(BathroomParser *Bp)
{
    free(Bp);
}

int fillBathroomParser(int argc, char const *argv[], BathroomParser *Bp)
{

    if (argc < 4)
    {
        return 1;
    }
    else
    {
        char aux[256];
        strcpy(aux, "");
        for (int i = 1; i < argc; i = i + 2)
        {
            //printf("Argv[%d] = %s\n", i, argv[i]);
            if (strcmp(argv[i], SEC_F) == 0)
            {
                Bp->secs_f = 1;
                if (isNumber(argv[i + 1]) == 0)
                {
                    Bp->nsecs = atoi(argv[i + 1]);
                }
                else
                {
                    return 1;
                }
            }
            else if (strcmp(argv[i], PLC_F) == 0)
            {
                Bp->place_f = 1;
                if (isNumber(argv[i + 1]) == 0)
                {
                    Bp->nplaces = atoi(argv[i + 1]);
                }
                else
                {
                    return 1;
                }
            }
            else if (strcmp(argv[i], THR_F) == 0)
            {
                Bp->thread_f = 1;
                if (isNumber(argv[i + 1]) == 0)
                {
                    Bp->nthreads = atoi(argv[i + 1]);
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                strcpy(Bp->fifoname, argv[i]);
            }
        }
        return 0;
    }
}

void printBathroomParser(BathroomParser *Bp)
{
    printf("Nsecs flag = %d || Nsecs = %d\n", Bp->secs_f, Bp->nsecs);
    printf("NPlaces flag = %d || NPlaces = %d\n", Bp->place_f, Bp->nplaces);
    printf("NThreads flag = %d || NThreads = %d\n", Bp->thread_f, Bp->nthreads);
    printf("FifoName = %s\n", Bp->fifoname);
}
