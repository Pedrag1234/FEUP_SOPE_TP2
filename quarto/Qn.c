#include "Qn.h"

//time is how long the server has been open, open_time is the max time the server CAN be open
int currentTime, open_time;

void * processRequest(void *arg) {
    Message * message = ( Message *) arg;
    int fd;
    char localFIFO[64];
    //order number
    int req_num = 0;

    genName(message->pid, message->tid, localFIFO);

    //reply message. recicles old message because most info is the same 
    message->pid = getpid();
    message->tid = pthread_self();

    if(currentTime < open_time) {
        //for some reason, not updating the place...
        printf("Hello\n");
        req_num++;
        message->pl = req_num;
        currentTime += message->dur;
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

    if (mkfifo(fifoname, 0660) != 0) {
        printf("Error creating public FIFO, exiting...\n");
        exit(1);
    }

    int fd = open(fifoname, O_RDONLY);

    Message message;
    while(read(fd, & message, sizeof(message) )>0) {
        pthread_t tid;
        //message must be void pointer because of the function create  
        pthread_create(& tid, NULL, processRequest, (void *) & message);
    }

    close(fd);
    unlink(fifoname);

    destroyBathroomParser(bp);
    exit(0);
}
