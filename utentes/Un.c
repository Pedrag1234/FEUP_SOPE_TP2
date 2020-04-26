#include "Un.h"

//this fd must be global to guarantee we are writing to the right place
int fd;

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

    write(fd, &message, sizeof(message));

    char localFifo[64];
    genName(message.pid, message.tid, localFifo);
    mkfifo(localFifo, 0666);
    fd_local = open(localFifo, O_RDONLY | O_NONBLOCK);

    while (read(fd_local, & message, sizeof(message)) <= 0) {
        usleep(10000);
    }

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
    printf("Hello\n");
    User * user = createUser();
    if (fillUser(user, argc, argv) != 0 || strlen(user->fifoname) == 0 || user->nsecs == 0)
    {
        printUsageClient();
        destroyUser(user);
        return -1;
    }

    printf("Hello1\n");
    
    printUser(user);
    int currentTime = 0, i = 1;
    int maxTime = user->nsecs;
    char fifoname[64];
    strcpy(fifoname, user->fifoname);

    printf("%s\n", fifoname);
   
    do
    {
        fd = open(fifoname, O_WRONLY | O_NONBLOCK);

        if (fd == -1) {
            printf("Connecting, please wait...\n");
            sleep(5);
        }

    } while (fd == -1);

    printf("Hello3\n");

    while (currentTime < maxTime)
    {
        pthread_t tid;
        pthread_create(&tid, NULL, sendRequest, (void *) & i); //no point in sending more of info, just the i value
        i++;
        currentTime += 1;
        usleep(1000000); //time in microsseconds
    }

    close(fd);

    destroyUser(user);
    pthread_exit(0);  
}
