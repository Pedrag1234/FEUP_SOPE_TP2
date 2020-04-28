#include "Un.h"

//this fd must be global to guarantee we are writing to the right place
int fd;
long int n_threads = 0;
pthread_t * threads;

void * sendRequest (void *arg) {
    int fd_local;
    time_t t;
    //seeding the rand func
    srand((unsigned) time(&t));

    Message message;
    message.i = *(int *) arg;
    message.pid = getpid();
    message.tid = pthread_self();
    //user sends -1 in place
    message.pl = -1;
    message.dur = (rand() % 500001) + 10000;

    printMsg(&message);

    write(fd, &message, sizeof(message));

    char localFifo[64];
    genName(message.pid, message.tid, localFifo);
    
    if (mkfifo(localFifo, 0660) != 0) {
        printf("Error creating private FIFO\n");
        exit(1);
    }

    fd_local = open(localFifo, O_RDONLY | O_NONBLOCK);


    while (read(fd_local, & message, sizeof(message)) <= 0) {
        usleep(10000);
    }
    printMsg(&message);

    if(message.pl != -1)
    {
        printf("IN!\n");
    }

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
    
    printUser(user);
    int currentTime = 0, i = 1;
    int maxTime = user->nsecs;
    char fifoname[64];
    strcpy(fifoname, user->fifoname);
   
    do
    {
        fd = open(fifoname, O_WRONLY | O_NONBLOCK);

        if (fd == -1) {
            printf("Connecting to PUBLIC FIFO, please wait...\n");
            sleep(5);
        }

    } while (fd == -1);

    threads = calloc(INITARRAY,sizeof(pthread_t));

    while (currentTime < maxTime)
    {

        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *) & i); //no point in sending more of info, just the id value
        if (n_threads < INITARRAY)
        {
            threads[n_threads] = tid;
        }
        else
        {
            threads = (pthread_t*) realloc(threads, sizeof(pthread_t) * (n_threads + 1));
            if (*threads)
            {
                threads[n_threads] = tid;
            }
            else
            {
                printf("Error reallocating thread array. Exiting ...\n");
                exit(1);
            }
        }
        n_threads++;


        i++;
        currentTime += 1;
        usleep(1000000); //time in microsseconds
    }

    for (int i = 0; i < n_threads; i++)
    {
        printf("I = %d || tid = %ld\n",i, threads[i]);
        pthread_join(threads[i],NULL);
    }

    close(fd);
    destroyUser(user);
    exit(0);  
}
