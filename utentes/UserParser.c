#include "UserParser.h"

User *createUser()
{
    User * user = (User *)malloc(sizeof(user));
    user->nsecs = 0;
    strcpy(user->fifoname, "");
    return user;
}

void destroyUser(User * user)
{
    free(user);
}

int fillUser(User * user, int argc, char const *argv[])
{   
    if(argc < 4)
    {
        return 1;
    }
    if(strcmp(argv[1], "-t") == 0)
    {
        user->nsecs = atoi(argv[2]);
        if (user->nsecs == 0)
        {
            return 1;
        }
        strcpy(user->fifoname, argv[3]);

    }
    else
    {
        return 1;
    }
    return 0;
}

void printUser(User * user)
{
    printf("Nsecs = %d\n", user->nsecs);
    printf("Fifoname = %s\n", user->fifoname);
}