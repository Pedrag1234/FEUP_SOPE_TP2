#include "Qn.h"

//time is how long the server has been open, open_time is the max time the server CAN be open
clock_t start,currentTime;
float open_time = 0;

pthread_t * threads;

long int  req_num = 0;
pthread_mutex_t req_num_lock;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
    int fd;
    char localFIFO[64];

    genName(message->pid, message->tid, localFIFO);

    //reply message. recicles old message because most info is the same 
    message->pid = getpid();
    message->tid = pthread_self();

    if((float)((currentTime - start)/CLOCKS_PER_SEC) < open_time) {
        
        //Increments request number 
        pthread_mutex_lock(&req_num_lock);
        req_num++;
        pthread_mutex_unlock(&req_num_lock);

        message->pl = req_num;
        usleep(message->dur * 1000); //use usleep bc time should be in the order of miliseconds and usleep is more precise
    }

    do {
        fd = open(localFIFO, O_WRONLY);

        if (fd == -1) {
            printf("Connecting to PRIVATE FIFO, please wait...\n");
            sleep(5);
        }

    } while (fd == -1);
    
    write(fd, message, sizeof(message));
    close(fd);
      
    return NULL;
}

int main(int argc, char const *argv[]) {
    start = clock();

    BathroomParser * bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, bp) != 0) || strlen(bp->fifoname) == 0 || bp->secs_f == 0)
    {
        printUsageServer();

        destroyBathroomParser(bp);
        return -1;
    }
    
    printBathroomParser(bp);

    open_time = bp->nsecs;
   
    char fifoname[64];
    strcpy(fifoname,bp->fifoname);

    if (mkfifo(fifoname, 0660) != 0) {
        printf("Error creating public FIFO, exiting...\n");
        exit(1);
    }

    if (pthread_mutex_init(&req_num_lock, NULL) != 0)
    {
        printf("Mutex init failed, exiting...\n");
        exit(1);
    }

    threads = calloc(INITARRAY,sizeof(pthread_t));

    int fd = open(fifoname, O_RDONLY);

    Message message;
    while(read(fd, & message, sizeof(message) )>0) {
        currentTime = clock();
        printMsg(&message);


        pthread_t tid;
        //message must be void pointer because of the function create  
        pthread_create(& tid, NULL, processRequest, (void *) & message);

        if (req_num < INITARRAY)
        {
            threads[req_num] = tid;
        }
        else
        {
            threads = (pthread_t*) realloc(threads, sizeof(pthread_t) * (req_num + 1));
            if (*threads)
            {
                threads[req_num] = tid;
            }
            else
            {
                printf("Error reallocating thread array. Exiting ...\n");
                exit(1);
            }
        }
    }
    printf("==========================================================\n");


    for (int i = 0; i < req_num; i++)
    {
        printf("I = %d || tid = %ld\n",i, threads[i]);
        pthread_join(threads[i],NULL);
    }
    

    close(fd);
    pthread_mutex_destroy(&req_num_lock);
    unlink(fifoname);

    destroyBathroomParser(bp);
    exit(0);
}
