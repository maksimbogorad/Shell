#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdint.h>

size_t lenstr(const char *s);
void stringCpy(char * dst, const char * src, size_t size);
void addSize(char ** string, size_t cur_len, uint16_t * newSize);

#endif