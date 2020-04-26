#include "Qn.h"

//pthread_t threads[MAXTHREADS];

//order number
int req_num = 0;

//time is how long the server has been open, open_time is the max time the server CAN be open
int currentTime, open_time;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
    int fd;
    char localFIFO[64];

    printMsg(message);

    genName(message->pid, message->tid, localFIFO);

    //reply message. recicles old message because most info is the same 
    message->pid = getpid();
    message->tid = pthread_self();

    if(currentTime < open_time) {
        req_num++;
        message->pl = req_num;
        currentTime += message->dur;
        usleep(message->dur * 1000); //use usleep bc time should be in the order of miliseconds and usleep is more precise
    }

    do {
        fd = open(localFIFO, O_WRONLY);
    } while (fd == -1);
    
    write(fd, message, sizeof(message));
    close(fd);
      
    return NULL;
}

int main(int argc, char const *argv[]) {
    BathroomParser * bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, bp) != 0) || strlen(bp->fifoname) == 0 || bp->secs_f == 0)
    {
        printUsageServer();

        destroyBathroomParser(bp);
        return -1;
    }
    
    printBathroomParser(bp);

    currentTime = 0;
    open_time = bp->nsecs;

    char fifoname[64];
    strcpy(fifoname,bp->fifoname);

    Message message;
    mkfifo(fifoname, 0660);
    //for testing use non block but after we should remove it
    int fd = open(fifoname, O_RDONLY);

    while(read(fd, & message, sizeof(message) )>0) {
        pthread_t tid;
        //message must be void pointer because of the function create  
        pthread_create(& tid, NULL, processRequest, (void *) & message);
    }

    close(fd);
    unlink(fifoname);

    destroyBathroomParser(bp);
    pthread_exit(0);
}
