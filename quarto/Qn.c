#include "Qn.h"

int open_time = 0, req_num = 0, isOpen = 0;
//int max_bathrooms = 0, *bathrooms;
pthread_mutex_t req_num_lock;
//pthread_mutex_t bathroom_in_lock, bathroom_out_lock;
pthread_t * threads;

//flags for threads and capacity
int threadsFlag = 0;
int placesFlag = 0;

sem_t nThreads;
sem_t nPlaces;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
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

    if((fd_local = open(localFIFO, O_WRONLY | O_NONBLOCK)) < 0) {
        logReg(&reply, "GAVUP");
        if(threadsFlag) sem_post(nThreads); //unlocks semaphore
        usleep(1000);
    }

    if(deltaTime() + message->dur * 1e-3  < open_time) {
        pthread_mutex_lock(&req_num_lock);
        req_num++;
        pthread_mutex_unlock(&req_num_lock);
        reply.pl = req_num;
        logReg(&reply, "ENTER");
    }
    else {
        isOpen = 1;
        reply.dur = -1;
        logReg(&reply, "2LATE");
    }

    if(reply.pl > 0){
        usleep(message->dur * 1000);
    }

    if(write(fd_local, &reply, sizeof(Message)) <= 0){
        logReg(&reply, 'GAVUP');
        if(threadsFlag) sem_post(&nThreads);
    }   
              
    if(!isOpen) logReg(&reply, "TIMUP");

    close(fd_local);
      
    if(threadsFlag) sem_post(&nThreads);
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

    //check wether or not there's a place/threads limit
    if(bp->nplaces > 0)
        placesFlag = 1;
    
    if(bp->nthreads > 0)
        threadsFlag = 0;

    open_time = bp->nsecs;
    char fifoname[64];
    strcpy(fifoname,bp->fifoname);

    if (mkfifo(fifoname, 0660) < 0) {
        perror("Error creating public FIFO, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&req_num_lock, NULL) != 0)
    {
        perror("Mutex init for request counter failed, exiting...\n");
        exit(1);
    }

    if((fd = open(fifoname, O_RDONLY | O_NONBLOCK)) == -1)
    {
        perror("Error opening public FIFO, exiting...");
        unlink(fifoname);
        exit(1);
    }
    
    threads = calloc(INITARRAY,sizeof(pthread_t));

    //initialize semaphores: sem_t, 0 is for threads, value for each semaphore (how many are 'allowed' to enter before being stopped)
    if(placesFlag) sem_init(&nPlaces, 0, bp->nplaces);
    if(threadsFlag) sem_init(&nThreads, 0, bp->nthreads);

    do {
        Message message;
        if(read(fd, & message, sizeof(Message)) > 0 ) {
            if(threadsFlag) sem_wait(&nThreads); //if threads are defined, lock thread semaphore
            int err = pthread_create(& tid, NULL, processRequest, (void *) & message);
            if (err != 0) break;    
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
                perror("Error reallocating thread array. Exiting ...\n");
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
