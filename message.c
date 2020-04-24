#include "message.h"

message *createMsg(int i, int dur, int pl)
{
    message *m = (message *)malloc(sizeof(message));

    m->i = i;
    m->pid = getpid();
    m->tid = pthread_self();
    m->dur = dur;
    m->pl = pl;

    return m;
}

void printMsg(message *m)
{
    printf("I = %d \t PID = %d \t TID = %d \t Dur = %d \t Pl = %d\n", m->i, m->pid, m->tid, m->dur, m->pl);
}

void destroyMsg(message *m)
{
    free(m);
}

void msg2string(message *m, char *str)
{
    char aux[256];
    strcpy(aux, "");

    sprintf(aux, "[%d,%d,%d,%d,%d]", m->i, m->pid, m->tid, m->dur, m->pl);
    strcpy(str, aux);
}

message *string2msg(char *str)
{
    message *m = (message *)malloc(sizeof(message));
    int i = 0, pid = 0, tid = 0, dur = 0, pl = 0;

    sscanf(str, "[%d,%d,%d,%d,%d]", &i, &pid, &tid, &dur, &pl);

    m->i = i;
    m->pid = pid;
    m->tid = tid;
    m->dur = dur;
    m->pl = pl;

    return m;
}
