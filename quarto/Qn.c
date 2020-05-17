#include "Qn.h"

int open_time = 0, req_num = 0, isOpen = 0;
pthread_mutex_t req_num_lock;
pthread_t *threads;

//flags for threads and capacity
int threadsFlag = 0;
int placesFlag = 0;

sem_t nThreads;
sem_t nPlaces;

//For limited places attribution
Queue queue;

// Processes requests sent by U2
void *processRequest(void *arg)
{
    /**
     *  resources are automatically released back to the system 
     * without the need for another thread to join with the terminated thread  
    */
    pthread_detach(pthread_self());
    Message *message = (Message *)arg;
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

    //Atempts to open response FIFO
    if ((fd_local = open(localFIFO, O_WRONLY | O_NONBLOCK)) < 0)
    {
        logReg(&reply, "GAVUP");
        if (threadsFlag)
            sem_post(&nThreads); //unlocks semaphore
        usleep(1000);
        pthread_exit(NULL);
    }


    int assignedPlace;
    //if the program is open or not
    if (deltaTime() + message->dur * 1e-3 < open_time)
    {
        
        if (placesFlag)
        {
            //Locks the creation of threads in case o reching limit of max threads
            sem_wait(&nPlaces);

            pthread_mutex_lock(&req_num_lock);
            //sets assigned place to the first free spot
            assignedPlace = add(&queue);
            pthread_mutex_unlock(&req_num_lock);
        }
        else
        {
            pthread_mutex_lock(&req_num_lock);
             //sets assigned place to the number of request
            assignedPlace = req_num;
            req_num++;
            pthread_mutex_unlock(&req_num_lock);
        }

        reply.pl = assignedPlace;
        logReg(&reply, "ENTER");
    }
    else
    {
        isOpen = 1;
        reply.dur = -1;
        logReg(&reply, "2LATE");
    }

    //wait for requested time
    if (reply.pl > 0)
    {
        usleep(message->dur * 1000);
    }

    //write response to the thread that requested
    if (write(fd_local, &reply, sizeof(Message)) <= 0)
    {
        logReg(&reply, "GAVUP");
        if (threadsFlag)
            sem_post(&nThreads);

        if (placesFlag)
        {
            //free place used and add it to the queue again
            pthread_mutex_lock(&req_num_lock);
            fillPlace(&queue, assignedPlace);
            pthread_mutex_unlock(&req_num_lock);
            sem_post(&nPlaces);
        }

        pthread_exit(NULL);
    }

    if (!isOpen)
        logReg(&reply, "TIMUP");

    close(fd_local);

    if (threadsFlag)
        sem_post(&nThreads);

    if (placesFlag)
    {
        pthread_mutex_lock(&req_num_lock);
        fillPlace(&queue, assignedPlace);
        pthread_mutex_unlock(&req_num_lock);
        sem_post(&nPlaces);
    }

    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    int fd;
    initClock();
    BathroomParser *bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, bp) != 0) || strlen(bp->fifoname) == 0 || bp->secs_f == 0)
    {
        printUsageServer();
        destroyBathroomParser(bp);
        exit(1);
    }

    //check wether or not there's a place/threads limit
    if (bp->nplaces > 0)
        placesFlag = 1;

    if (bp->nthreads > 0)
        threadsFlag = 0;

    open_time = bp->nsecs;
    char fifoname[64];
    strcpy(fifoname, bp->fifoname);

    //Open and create FIFO, Mutexes and Semaphores
    if (mkfifo(fifoname, 0660) < 0)
    {
        perror("Error creating public FIFO, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&req_num_lock, NULL) != 0)
    {
        perror("Mutex init for request counter failed, exiting...\n");
        exit(1);
    }

    if ((fd = open(fifoname, O_RDONLY | O_NONBLOCK)) == -1)
    {
        perror("Error opening public FIFO, exiting...");
        unlink(fifoname);
        exit(1);
    }


    //initialize semaphores: sem_t, 0 is for threads, value for each semaphore (how many are 'allowed' to enter before being stopped)
    //also initialize queue for place maintenance
    if (placesFlag)
    {
        sem_init(&nPlaces, 0, bp->nplaces);
        queue = createQueue(bp->nplaces);
        fill(&queue);
    }

    if (threadsFlag)
        sem_init(&nThreads, 0, bp->nthreads);

    do
    {
        Message message;
        if (read(fd, &message, sizeof(Message)) > 0)
        {
            if (threadsFlag)
                sem_wait(&nThreads); //if threads are defined, lock thread semaphore

            //Create threads
            pthread_t tid;
            int err = pthread_create(&tid, NULL, processRequest, (void *)&message);
            if (err != 0)
                break;
        }

    } while (deltaTime() < open_time);


    isOpen = 1;

    //Free resources allocated by program
    close(fd);

    pthread_mutex_destroy(&req_num_lock);

    sem_destroy(&nPlaces);
    sem_destroy(&nThreads);

    unlink(fifoname);
    destroyBathroomParser(bp);
    pthread_exit(0);
}
