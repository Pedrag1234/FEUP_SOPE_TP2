#ifndef _UTILS_H_
#define _UTILS_H_

#include <string.h>
#include <ctype.h>

/**
 * @brief: checks if a string is made out entirely of digits
 * 
 * @param string: string to be checked
 * 
 * @return: returns 0 if is made entirely out of digits and 1 if not 
 */
int isNumber(const char *string);

/**
 * @brief: gets a substring from a string
 * 
 * @param input: string to be divided
 * @param offset: index to begin the division 
 * @param length: length of the desired substring
 * @param dest: destination of the result
 * 
 * @return null if offset + lenght > input_length and dest if everything went ok
  */
char *subString(const char *input, int offset, int len, char *dest);

void copyArray(int *dst, int *src);

#endif
