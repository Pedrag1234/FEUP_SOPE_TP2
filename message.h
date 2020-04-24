#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

typedef struct message
{
    int i;
    int pid;
    int tid;
    int dur;
    int pl;
} message;

/**
 * @brief: creates a struct of type message
 * 
 * @param i: request id
 * @param dur: duration
 * @param pl: place
 * 
 * @return: pointer to the created struct 
 */
message *createMsg(int i, int dur, int pl);

/**
 * @brief: prints a struct of type message
 * 
 * @param m: the struct to print
 */
void printMsg(message *m);

/**
 * @brief: destroys a struct of type message
 * 
 * @param m: the struct to be destroyed
 */
void destroyMsg(message *m);

/**
 * @brief: parses a struct message to a string 
 * 
 * @param m: struct to be parse
 * @param str: the result string
 */
void msg2string(message *m, char *str);

/**
 * @brief: parses a string to a struct message 
 * 
 * @param str: the string to be parsed
 * 
 * @return returns an instance of message
 */
message *string2msg(char *str);

#endif
