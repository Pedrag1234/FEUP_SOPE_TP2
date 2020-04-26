#include "utils.h"

int isNumber(const char *string)
{
    int s_size = strlen(string);

    for (int i = 0; i < s_size; i++)
    {
        if (isdigit(string[i]) == 0)
            return 1;
    }
    return 0;
}

char *subString(const char *input, int offset, int len, char *dest)
{
    int input_len = strlen(input);

    if (offset + len > input_len)
    {
        return NULL;
    }

    strncpy(dest, input + offset, len);
    return dest;
}

void copyArray(int *dst, int *src)
{
    for (int i = 0; i < 1024; i++)
    {
        dst[i] = src[i];
    }
}

void logReg(int tid, int i, int dur, int pl, char *oper)
{
    char *message;
    message = (char *)malloc(128 * sizeof(char));
    time_t t = time(NULL);

    sprintf(message, "%ld ; %d ; %d ; %d ; %d ; %d ; %s\n", t, i, getpid(), tid, dur, pl, oper);
    write(STDOUT_FILENO, message, strlen(message));
}

void genName(pid_t pid, pthread_t tid, char *str)
{
    char spid[32], stid[32];
    char fifo[64] = "/tmp/";
    sprintf(spid, "%d", pid);
    sprintf(stid, "%ld", tid);
    strcat(fifo, spid);
    strcat(fifo, ".");
    strcat(fifo, stid);

    strcpy(str, fifo);
}

void printUsageClient()
{
    printf("Usage: ./Un <-t nsecs> fifoname\n");
}

void printUsageServer()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}