#include "message.h"

void printMsg(Message *m)
{
    printf("I = %d \t PID = %d \t TID = %ld \t Dur = %d \t Pl = %d\n", m->i, m->pid, m->tid, m->dur, m->pl);
}

void destroyMsg(Message *m)
{
    free(m);
}
