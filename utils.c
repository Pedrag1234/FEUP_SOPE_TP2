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

void logReg(Message * m, char *oper)
{
    fprintf(stdout, "%.10ld ; %.4d ; %.6d ; %.6ld ; %.6d ; %d ; %s\n", time(NULL), m->i, m->pid , m->tid, m->dur, m->pl, oper);
    fflush(stdout);
}

void genName(pid_t pid, long tid, char *str)
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

void initClock() {
    gettimeofday(&start, NULL);
    delta = 0;
}

double deltaTime() {
    gettimeofday(&currentTime, NULL);

    delta = (currentTime.tv_sec - start.tv_sec) * 1e6;
    delta = (delta + (currentTime.tv_usec - start.tv_usec)) * 1e-6;

    return delta;
}