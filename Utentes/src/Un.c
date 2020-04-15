#include "Un.h"

void printUsage()
{
    printf("Usage: ./Un <-t nsecs> fifoname\n");
}

int main(int argc, char const *argv[])
{
    user *us = createUser();
    if (fillUser(us, argc, argv) != 0 || strlen(us->fifoname) == 0 || us->nsecs == 0)
    {
        printUsage();
    }
    else
    {
        printUser(us);
    }

    destroyUser(us);
    return 0;
    
}
