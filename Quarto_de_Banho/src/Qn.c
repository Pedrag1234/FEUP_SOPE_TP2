#include "Qn.h"

pthread_t threads[MAXTHREADS];

int req_num = 0;

void *processRequest(void *arg)
{
    char *request = (char *)arg;

    struct Message *req = string2msg(request);
    printf("Request:\n");
    printMsg(req);

    req_num++;

    char prvt_fifoname[261];
    genName(req->pid,req->tid,prvt_fifoname);

    int fd = open(prvt_fifoname, O_WRONLY);

    if (fd < 0)
    {
        printf("Could not open FIFO. Exiting ...\n");
        return NULL;
    }
    

    struct Message *res = createMsg(req->i,req->dur,req_num);
    char s_res[256];
    msg2string(res,s_res);

    write(fd,s_res,sizeof(s_res));

    sleep(res->dur);
    //printf("Response:\n");
    //printMsg(res);
    
    if (close(fd) != 0)
    {
        printf("Could not close FIFO. Exiting ...\n");
        return NULL;
    }
    
    
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
        //pthread_t thread;
        time_t start, end;
        double elapsed;

        char fifoname[261];
        strcpy(fifoname, "");

        sprintf(fifoname, "/tmp/%s", Bp->fifoname);
        mkfifo(fifoname, 0666);

        //TEST
        mkfifo("/tmp/2.3", 0666);

        int fd = open(fifoname, O_RDONLY | O_NONBLOCK);
        
        //TEST
        int fd_test = open("/tmp/2.3", O_RDONLY | O_NONBLOCK);

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
            strcpy(request,"[1,2,3,4,5]");
            if (strlen(request) > 0)
            {
                int i = 0;
                while (pthread_create(&threads[req_num], NULL, processRequest, request))
                {
                    if (i == 5)
                    {
                        printf("Error creating thread. Exiting ...\n");
                        exit(1);
                    }
                    i++;
                }
            }
            //TEST
            read(fd_test, request, 256);
            printf("RESPONSE = %s\n",request);
        }
        for (int i = 0; i < req_num; i++)
        {
            pthread_join(threads[i],NULL);
        }
        
    }

    destroyBathroomParser(Bp);
    return 0;
}

void printUsage()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}
