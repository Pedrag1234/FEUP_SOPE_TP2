#ifndef _USERPARSER_H_
#define _USERPARSER_H_

#include <stdlib.h>
#include <string.h> 
#include <stdio.h>

typedef struct
{
    int nsecs;
    char fifoname[256];
} User;

void destroyUser(User * user);
int fillUser(User * user, int argc, char const *argv[]);
void printUser(User * user);
User *createUser(); 

#endif