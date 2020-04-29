#ifndef _UTILS_H_
#define _UTILS_H_

#include <sys/types.h>
#include <sys/time.h>

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "message.h"

struct timeval start;
struct timeval currentTime;
double delta;

/**
 * @brief: checks if a string is made out entirely of digits
 * 
 * @param string: string to be checked
 * 
 * @return: returns 0 if is made entirely out of digits and 1 if not 
 */
int isNumber(const char *string);

/**
 * @brief: gets a substring from a string
 * 
 * @param input: string to be divided
 * @param offset: index to begin the division 
 * @param length: length of the desired substring
 * @param dest: destination of the result
 * 
 * @return null if offset + lenght > input_length and dest if everything went ok
  */
char *subString(const char *input, int offset, int len, char *dest);

void copyArray(int *dst, int *src);

/**
 * @brief: writes the log msgs to the fd. 
 * 
 * @param m: message struct containing necessary info
 * @param oper: executed op that led to the writing in the fd
 * 
 * @return: none
 * 
*/
void logReg(Message * m, char *oper);

/**
 * @brief: creates name for fifo according to spec
 * 
 * @param pid: process id
 * @param tid: thread id
 * 
 * @return: name generated
 * 
*/
void genName(pid_t pid, long tid, char *str);

/**
 * @brief: prints how to use the command for the Client Program
 */
void printUsageClient();

/**
 * @brief: prints how to use the command for the Server Program
 * 
*/
void printUsageServer();

/**
 * @brief: initializes internal clock
 * 
*/
void initClock();

/**
 * @brief: returns time passed since the beginning of execution
 * 
*/
double deltaTime(); 

#endif
