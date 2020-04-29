#include "Qn.h"

//time is how long the server has been open, open_time is the max time the server CAN be open
int open_time = 0, req_num = 0, isOpen = 0;
pthread_mutex_t req_num_lock;
pthread_t * threads;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
    int fd_local;
    char localFIFO[64];

    printMsg(message);

    genName(message->pid, message->tid, localFIFO);

    Message reply; 
    reply.pid = getpid();
    reply.tid = pthread_self();
    reply.dur = message->dur;
    reply.i = message->i;
    reply.pl = -1;
    //---log RECEIVED---//

    while((fd_local = open(localFIFO, O_WRONLY)) < 0) {
        //---log GAVEUP---//
        usleep(1000);
    }

    if(deltaTime() + message->dur * 1e-3  < open_time) {
        pthread_mutex_lock(&req_num_lock);
        req_num++;
        pthread_mutex_unlock(&req_num_lock);
        reply.pl = req_num;
        //---log ENTER---//
    }
    else {
        isOpen = 1;
        reply.dur = -1;
        //---log TOOLATE---//
    }

    //if(!isOpen)
        //---log TIMEUP---//
    
    usleep(message->dur * 1000);
    write(fd_local, &reply, sizeof(Message));
    close(fd_local);
      
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

    if((fd = open(fifoname, O_RDONLY)) == -1)
    {
        perror("Error opening public FIFO, exiting...");
        unlink(fifoname);
        exit(1);
    }

    threads = calloc(INITARRAY,sizeof(pthread_t));

    do {
        Message message;
        if(read(fd, & message, sizeof(Message)) > 0 ) {
            pthread_create(& tid, NULL, processRequest, (void *) & message);
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
