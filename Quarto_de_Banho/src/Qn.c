#include "Qn.h"

void *processRequest(void *arg)
{
    char *request = (char *)arg;

    return NULL;
}

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
        pthread_t thread;
        time_t start, end;
        double elapsed;

        char fifoname[261];
        strcpy(fifoname, "");

        sprintf(fifoname, "/tmp/%s", Bp->fifoname);
        mkfifo(fifoname, 0666);

        int fd = open(fifoname, O_RDONLY | O_NONBLOCK);

        if (fd < 0)
        {
            printf("Could not open FIFO. Exiting ...\n");
            exit(1);
        }

        char request[256];

        time(&start);
        time(&end);

        elapsed = difftime(end, start);
        while (elapsed <= Bp->nsecs)
        {
            time(&end);
            elapsed = difftime(end, start);
            read(fd, request, 256);
            if (strlen(request) > 0)
            {
                int i = 0;
                while (pthread_create(&thread, NULL, processRequest, request))
                {
                    if (i == 5)
                    {
                        printf("Error creating thread. Exiting ...\n");
                        exit(1);
                    }
                    i++;
                }
            }
        }
    }

    destroyBathroomParser(Bp);
    return 0;
}

void printUsage()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}
