#ifndef _USERPARSER_H_
#define _USERPARSER_H_

#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

typedef struct
{
    int nsecs;
    char fifoname[256];
} user;

void destroyUser(user *us);
int fillUser(user *us, int argc, char const *argv[]);
void printUser(user *us);
user *createUser(); 

#endif