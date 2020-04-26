#include "message.h"

Message *createMsg(int i, int dur, int pl)
{
    Message *m = (Message *)malloc(sizeof(Message));

    m->i = i;
    m->pid = getpid();
    m->tid = pthread_self();
    m->dur = dur;
    m->pl = pl;

    return m;
}

void printMsg(Message *m)
{
    printf("I = %d \t PID = %d \t TID = %ld \t Dur = %d \t Pl = %d\n", m->i, m->pid, m->tid, m->dur, m->pl);
}

void destroyMsg(Message *m)
{
    free(m);
}

void msg2string(Message *m, char *str)
{
    char aux[256];
    strcpy(aux, "");

    sprintf(aux, "[%d,%d,%ld,%d,%d]", m->i, m->pid, m->tid, m->dur, m->pl);
    strcpy(str, aux);
}

Message *string2msg(char *str)
{
    Message *m = (Message *)malloc(sizeof(Message));
    int i = 0, pid = 0, tid = 0, dur = 0, pl = 0;

    sscanf(str, "[%d,%d,%d,%d,%d]", &i, &pid, &tid, &dur, &pl);

    m->i = i;
    m->pid = pid;
    m->tid = tid;
    m->dur = dur;
    m->pl = pl;

    return m;
}
