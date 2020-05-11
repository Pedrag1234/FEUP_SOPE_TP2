#include "request_list.h"

Queue createQueue(int size) { 
    Queue queue; 
    queue.capacity = size; 
    queue.front = 0;
    queue.rear = size - 1;
    queue.size = 0;  
    queue.array = (int *) malloc(queue.capacity * sizeof(int)); 

    return queue; 
}

int emptyQ(Queue* queue) {  
    return (queue->size == 0); 
} 

int fullQ(Queue* queue) {  
    return (queue->size == queue->capacity);  
} 

void fillPlace(Queue * queue, int client) {
    if (!fullQ(queue)){
        queue->rear = (queue->rear + 1) % queue->capacity; 
        queue->array[queue->rear] = client; 
        queue->size++;
    }
}

int add(Queue * queue) {
    if (emptyQ(queue)) 
        return 0;

    int item = queue->array[queue->front]; 
    queue->front = (queue->front + 1) % queue->capacity; 
    queue->size--; 

    return item;
}

void fill(Queue * queue) {
    for(int i = 1;i <= queue->capacity;i++)
        fillPlace(queue, i);
}
