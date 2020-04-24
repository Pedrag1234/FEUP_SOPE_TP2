#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief: writes the log msgs to the fd. 
 * 
 * @param tid: thread id
 * @param i: request id (or number)
 * @param dur: duration of usage
 * @param pl: assigned place in Qc
 * @param oper: executed op that led to the writing in the fd
 * 
 * @return: none
 * 
*/
void log(int tid, int i, int dur, int pl, char *oper);


/**
 * @brief: creates name for fifo according to spec
 * 
 * @param pid: process id
 * @param tid: thread id
 * 
 * @return: name generated
 * 
*/
char * genName(int pid, int tid);

#endif