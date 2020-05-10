#ifndef _UN_H_
#define _UN_H_

#define MAX_TIME_BETWEEN_THREADS 999
#define MIN_TIME_BETWEEN_THREADS 200

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "UserParser.h"
#include "../utils.h"
#include "../message.h"

#define INITARRAY 1000000

#endif
