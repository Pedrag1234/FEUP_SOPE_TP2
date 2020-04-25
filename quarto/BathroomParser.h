#ifndef _BATHROOMPARSER_H_
#define _BATHROOMPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils.h"

#define MAX_FIFONAME 256

#define SEC_F "-t"
#define PLC_F "-l"
#define THR_F "-n"

typedef struct
{
    int secs_f;
    int nsecs;
    int place_f;
    int nplaces;
    int thread_f;
    int nthreads;
    char fifoname[MAX_FIFONAME];

} BathroomParser;

/**
 * @brief: Creates a struct of type BathroomParser
 * 
 * @return: Returns a pointer to the struct 
 */
BathroomParser *createBathroomParser();

/**
 * @brief: Destroys a struct of type BathroomParser
 */
void destroyBathroomParser(BathroomParser *Bp);

/**
 * @brief: Fills a struct of type BathroomParser
 * 
 * @param argc: Number args passed through cmd
 * @param argv: Strings of the args passed
 * @param Bp: struct to be filled
 * 
 * @return: 0 if it was a success and 1 if not
 */
int fillBathroomParser(int argc, char const *argv[], BathroomParser *Bp);

/**
 * @brief: Prints a struct of type BathroomParser
 */
void printBathroomParser(BathroomParser *Bp);

#endif