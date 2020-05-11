#ifndef _REQUEST_LIST_H_
#define _REQUEST_LIST_H_

#include <stdio.h> 
#include <stdlib.h> 
#include <limits.h> 

typedef struct{
    int front, rear, size, capacity; 
    int * array; 
} Queue; 

Queue createQueue(int size);

int emptyQ(Queue * queue);

int fullQ(Queue * queue);

void fillPlace(Queue * queue, int client);

int add(Queue * queue);

void fill(Queue * queue);

#endif
