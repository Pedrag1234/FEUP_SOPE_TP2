#include "Qn.h"

//time is how long the server has been open, open_time is the max time the server CAN be open

 
int open_time = 0, req_num = 0, isOpen = 0, max_bathrooms = 0, *bathrooms;
pthread_mutex_t req_num_lock, bathroom_in_lock, bathroom_out_lock;
pthread_t * threads;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
    int pl = 0;
    int fd_local;
    char localFIFO[64];

    logReg(message, "RECVD");

    genName(message->pid, message->tid, localFIFO);

    Message reply; 
    reply.pid = getpid();
    reply.tid = pthread_self();
    reply.dur = message->dur;
    reply.i = message->i;
    reply.pl = -1;

    while((fd_local = open(localFIFO, O_WRONLY)) < 0) {
        logReg(&reply, "GAVUP");
        usleep(1000);
    }

    if(deltaTime() + message->dur * 1e-3  < open_time) {
        pthread_mutex_lock(&req_num_lock);
        req_num++;
        pthread_mutex_unlock(&req_num_lock);

        if (max_bathrooms == 0)
        {
            reply.pl = req_num;
            logReg(&reply, "ENTER");
        }
        else
        {
            int ocp = 0;
            for (int i = 0; i < max_bathrooms; i++)
            {
                if (bathrooms[i] == 1)
                {
                    ocp++;
                }
                
            }
            
            if (ocp < max_bathrooms)
            {
                pthread_mutex_lock(&bathroom_in_lock);
                for (int i = 0; i < max_bathrooms; i++)
                {
                    if (bathrooms[i] == 0)
                    {
                        pl = i;
                        bathrooms[i] = 1;
                        reply.pl = i + 1;
                        logReg(&reply, "ENTER");
                        break;
                    }
                }
                pthread_mutex_unlock(&bathroom_in_lock);
            }
            else
            {
                reply.pl = -1;
                logReg(&reply, "ENTER");
            }
            
            
        }

    }
    else {
        isOpen = 1;
        reply.dur = -1;
        logReg(&reply, "2LATE");
    }

    if(reply.pl > 0){
        usleep(message->dur * 1000);
    }
        
        
    if(!isOpen) logReg(&reply, "TIMUP");
    
    write(fd_local, &reply, sizeof(Message));
    close(fd_local);

    pthread_mutex_lock(&bathroom_out_lock);
    if (reply.pl != -1 && max_bathrooms != 0){
        bathrooms[pl] = 0;
    } 
    pthread_mutex_unlock(&bathroom_out_lock);
      
    return NULL;
}

int main(int argc, char const *argv[]) {
    int fd;
    pthread_t tid;
    initClock();
    BathroomParser * bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, bp) != 0) || strlen(bp->fifoname) == 0 || bp->secs_f == 0) {
        printUsageServer();
        destroyBathroomParser(bp);
        exit(1);
    }
    
    printBathroomParser(bp);
    open_time = bp->nsecs;
    char fifoname[64];
    strcpy(fifoname,bp->fifoname);

    if (mkfifo(fifoname, 0660) < 0) {
        printf("Error creating public FIFO, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&req_num_lock, NULL) != 0)
    {
        printf("Mutex init failed, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&bathroom_in_lock, NULL) != 0)
    {
        printf("Mutex init failed, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&bathroom_out_lock, NULL) != 0)
    {
        printf("Mutex init failed, exiting...\n");
        exit(1);
    }

    if((fd = open(fifoname, O_RDONLY | O_NONBLOCK)) == -1)
    {
        perror("Error opening public FIFO, exiting...");
        unlink(fifoname);
        exit(1);
    }

    if (bp->place_f != 0 && bp->nplaces != 0)
    {
        max_bathrooms = bp->nplaces;
        bathrooms = calloc(max_bathrooms,sizeof(int));
        for (int i = 0; i < max_bathrooms; i++)
        {
            bathrooms[i] = 0;
        }
        
    }
    
    threads = calloc(INITARRAY,sizeof(pthread_t));

    do {
        Message message;
        if(read(fd, & message, sizeof(Message)) > 0 ) {
            int err = pthread_create(& tid, NULL, processRequest, (void *) & message);
            if (err != 0)
            {
                break;
            }
            
        }

        if (req_num < INITARRAY) {
            threads[req_num] = tid;
        }
        else {
            threads = (pthread_t*) realloc(threads, sizeof(pthread_t) * (req_num + 1));
            if (*threads) {
                threads[req_num] = tid;
            }
            else {
                printf("Error reallocating thread array. Exiting ...\n");
                exit(1);
            }
        }

    } while(deltaTime() < open_time);

    for (int i = 0; i < req_num; i++) {
        pthread_join(threads[i],NULL);
    }
    
    isOpen = 1;
    close(fd);
    pthread_mutex_destroy(&req_num_lock);
    unlink(fifoname);
    destroyBathroomParser(bp);
    pthread_exit(0);
}
