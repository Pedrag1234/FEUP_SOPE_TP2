#ifndef _BATHROOMPARSER_H_
#define _BATHROOMPARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

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

BathroomParser *createBathroomParser();

void destroyBathroomParser(BathroomParser *Bp);

int fillBathroomParser(int argc, char const *argv[], BathroomParser *Bp);

void printBathroomParser(BathroomParser *Bp);

#endif