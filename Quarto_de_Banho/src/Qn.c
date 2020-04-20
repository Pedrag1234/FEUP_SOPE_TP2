#include "Qn.h"

int main(int argc, char const *argv[])
{
    BathroomParser *Bp = createBathroomParser();

    if ((fillBathroomParser(argc, argv, Bp) != 0) || strlen(Bp->fifoname) == 0 || Bp->secs_f == 0)
    {
        printUsage();
    }
    else
    {
        printBathroomParser(Bp);
    }

    message *m = createMsg(1, 2, 30);
    char s[256];

    msg2string(m, s);
    printf("%s\n", s);

    message *m2 = string2msg(s);
    printMsg(m2);

    destroyBathroomParser(Bp);
    return 0;
}

void printUsage()
{
    printf("./Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
}
