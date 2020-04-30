#include "Un.h"

//this fd must be global to guarantee we are writing to the right place
int fd;
long int n_threads = 0;
pthread_t * threads;

void * sendRequest (void *arg) {
    int fd_local;

    Message * message = (Message *) arg;
    message->pid = getpid();
    message->tid = pthread_self();

    if(fd == -1) {
        logReg(message, "CLOSD");
        return NULL;
    }

    logReg(message, "IWANT");
    write(fd, message, sizeof(Message));

    char localFifo[64];
    genName(message->pid, message->tid, localFifo);
    
    if (mkfifo(localFifo, 0660) < 0) {
        printf("Error creating private FIFO, exiting...\n");
        exit(1);
    }

    if((fd_local = open(localFifo, O_RDONLY)) < 0) {
        perror("Error OPENING private FIFO, exiting...\n");
        exit(1);
    }

    if (read(fd_local, message, sizeof(Message)) < 0) {
        logReg(message, "FAILD");
        return NULL;
    }

    if(message->pl == -1 && message->dur == -1) {
       logReg(message, "CLOSD");
    }
    else logReg(message, "IAMIN");

    close(fd_local);
    unlink(localFifo);
    return NULL;
}

int main(int argc, char const *argv[])
{
    User * user = createUser();
    if (fillUser(user, argc, argv) != 0 || strlen(user->fifoname) == 0 || user->nsecs == 0)
    {
        printUsageClient();
        destroyUser(user);
        return -1;
    }
    
    int i = 1;
    initClock();
    int maxTime = user->nsecs;
    char fifoname[64];
    strcpy(fifoname, user->fifoname);
    srand(time(NULL));

    fd = open(fifoname, O_WRONLY);

    threads = calloc(INITARRAY,sizeof(pthread_t));

    while (deltaTime() < maxTime)
    {
        Message message;
        message.i = i++;
        message.pl = -1;
        message.dur = (rand() % 50) + 1;

        pthread_t tid;
        int err = pthread_create(&tid, NULL, sendRequest, (void *) & message);
        if (err != 0)
        {
            break;
        }
        

        if (n_threads < INITARRAY) {
            threads[n_threads] = tid;
        }
        else {
            threads = (pthread_t*) realloc(threads, sizeof(pthread_t) * (n_threads + 1));
            if (*threads) {
                threads[n_threads] = tid;
            }
            else {
                printf("Error reallocating thread array. Exiting ...\n");
                exit(1);
            }
        }
        n_threads++;

        usleep(50000); //time in microsseconds
    }

    for (int i = 0; i < n_threads; i++){
        pthread_join(threads[i],NULL);
    }

    close(fd);
    destroyUser(user);
    pthread_exit(0);  
}
