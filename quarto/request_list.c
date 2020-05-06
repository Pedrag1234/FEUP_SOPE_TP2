#include "request_list.h"

request_list *createList()
{
    request_list *l = (request_list *)malloc(sizeof(request_list));

    l->size = 0;
    for (int i = 0; i < 1024; i++)
    {
        strcpy(l->requests[i], "");
    }

    return l;
}

int addRequest(const char *req, request_list *r)
{
    if (r->size == 1024)
    {
        return 1;
    }
    else
    {
        strcpy(r->requests[r->size], req);
        r->size += 1;
        return 0;
    }
}

void requestProcessed(request_list *r)
{
    r->size -= 1;
    for (int i = 0; i < r->size; i++)
    {
        if (i == 1023)
        {
            strcpy(r->requests[i], "");
        }
        else
        {
            strcpy(r->requests[i], r->requests[i + 1]);
        }
    }
}

void destroyList(request_list *r)
{
    free(r);
}
