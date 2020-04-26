#include "Qn.h"



pthread_t *threads;

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

    int fd = open(prvt_fifoname, O_WRONLY | O_NONBLOCK);

    if (fd < 0)
    {
        printf("Thread could not open FIFO. Exiting ...\n");
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
        printf("Thread could not close FIFO. Exiting ...\n");
        return NULL;
    }
    
    
    return NULL;
}

int main(int argc, char const *argv[])
{
    BathroomParser *Bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, Bp) != 0) || strlen(Bp->fifoname) == 0 || Bp->secs_f == 0)
    {
        printUsageServer();
    }
    else
    {
        printBathroomParser(Bp);
        time_t start, end;
        double elapsed;


        char fifoname[261];
        strcpy(fifoname, "");

        sprintf(fifoname, "/tmp/%s", Bp->fifoname);
        mkfifo(fifoname, 0666);

        int fd = open(fifoname, O_RDONLY | O_NONBLOCK);

        if (fd < 0)
        {
            printf("Could not open public FIFO. Exiting ...\n");
            exit(1);
        }

        threads = calloc(INITARRAY,sizeof(pthread_t));

        char request[256];

        time(&start);
        time(&end);
        int m = 0;
        elapsed = difftime(end, start);
        
        while (elapsed <= Bp->nsecs)
        {
            m++;
            time(&end);
            elapsed = difftime(end, start);
            read(fd, request, 256);

            //If a request exists
            if (strlen(request) > 0)
            {
                int i = 0;
                int err = -1;
                pthread_t p;
                
                //Creates threads
                do{
                    
                    err = pthread_create(&p, NULL, processRequest, request);
                    i++;
                    if (i == 5)
                    {
                        printf("Error creating threads\n");
                        exit(1);
                    }
                } while (err != 0);
                
                if (req_num < INITARRAY)
                {
                    threads[req_num] = p;
                }
                else
                {
                    threads = (pthread_t*) realloc(threads, sizeof(pthread_t) * (req_num + 1));
                
                    if (*threads)
                    {
                        threads[req_num] = p;
                    }
                    else
                    {
                        printf("Error reallocating thread array. Exiting ...\n");
                        exit(1);
                    }
                }
            }

        }
        //Joins all threads
        for (int j = 0; j < req_num; j++)
        {
            pthread_join(threads[j],NULL);

        }
        
    }

    destroyBathroomParser(Bp);
    return 0;
}
