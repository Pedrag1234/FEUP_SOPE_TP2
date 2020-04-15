#include "utils.h"

int isNumber(const char *string)
{
    int s_size = strlen(string);

    for (int i = 0; i < s_size; i++)
    {
        if (isdigit(string[i]) == 0)
            return 1;
    }
    return 0;
}

char *subString(const char *input, int offset, int len, char *dest)
{
    int input_len = strlen(input);

    if (offset + len > input_len)
    {
        return NULL;
    }

    strncpy(dest, input + offset, len);
    return dest;
}

void copyArray(int *dst, int *src)
{
    for (int i = 0; i < 1024; i++)
    {
        dst[i] = src[i];
    }
}