#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
 * @param tid: thread id
 * @param i: request id (or number)
 * @param dur: duration of usage
 * @param pl: assigned place in Qc
 * @param oper: executed op that led to the writing in the fd
 * 
 * @return: none
 * 
*/
void logReg(int tid, int i, int dur, int pl, char *oper);

/**
 * @brief: creates name for fifo according to spec
 * 
 * @param pid: process id
 * @param tid: thread id
 * 
 * @return: name generated
 * 
*/
void genName(int pid, int tid, char *str);

/**
 * @brief: prints how to use the command for the Client Program
 */
void printUsageClient();

/**
 * @brief: prints how to use the command for the Server Program
 * 
*/
void printUsageServer();

#endif
