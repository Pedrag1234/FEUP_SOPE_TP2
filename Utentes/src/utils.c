// #include "utils.h"

// void log(int tid, int i, int dur, int pl, char *oper) {
//     char* message;
//     message = (char*) malloc (128 * sizeof(char));
//     time_t t = time(NULL);

//     sprintf(message, "%ld ; %d ; %d ; %d ; %d ; %d ; %s\n", t, i, getpid(), tid, dur, pl, oper);
//     write(STDOUT_FILENO, message, strlen(message));
// }

// char * genName(int pid, int tid)
// {
//     char * spid, stid;
//     char fifo[64] = "/tmp/";
//     sprintf(spid, "%d", pid);
//     sprintf(stid, "%d", tid);
//     strcat(fifo, spid);
//     strcat(fifo, ".");
//     strcat(fifo, stid);

//     return fifo;
// }