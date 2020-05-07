#ifndef _REQUEST_LIST_H_
#define _REQUEST_LIST_H_

#include "../message.h"

typedef struct
{
    int size;
    char requests[1000000][256];
} request_list;

/**
 * @brief: Creates an empty list of requests
 * 
 * @return: returns a pointer to the list of requests  
 */
request_list *createList();

/**
 * @brief: Adds a request to be processed later
 * 
 * @param req: the string of the request
 * @param r: the list to add the request
 * 
 * @return: 0 if it was added and -1 if not
 */
int addRequest(const char *req, request_list *r);

/**
 * @brief: Deletes the first element of the list
 * 
 * @param r: the list to remove the request
 */
void requestProcessed(request_list *r);

/**
 * @brief: Destroys the list
 * 
 * @param r: the list to be destroyed
 */
void destroyList(request_list *r);

#endif
