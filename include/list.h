#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct List 
{
    char *part;
    int in_quotes;
    struct List *next;
} List;

void initList(List **node);
int isEmptyList(List *node);
void addElem(List **node, const char *str, uint16_t stringSize, int is_in_quotes);
void freeList(List *list);

#endif