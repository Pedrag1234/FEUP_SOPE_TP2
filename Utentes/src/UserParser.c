#include "UserParser.h"

user *createUser()
{
    user *us = (user *)malloc(sizeof(user));
    us->nsecs = 0;
    strcpy(us->fifoname, "");
    return us;
}

void destroyUser(user *us)
{
    free(us);
}

int fillUser(user *us, int argc, char const *argv[])
{   
    if(argc < 4)
    {
        return 1;
    }
    if(strcmp(argv[1], "-t") == 0)
    {
        us->nsecs = atoi(argv[2]);
        if (us->nsecs == 0)
        {
            return 1;
        }
        strcpy(us->fifoname, argv[3]);

    }
    else
    {
        return 1;
    }
    return 0;
}

void printUser(user *us)
{
    printf("Nsecs = %d\n", us->nsecs);
    printf("Fifoname = %s\n", us->fifoname);
}