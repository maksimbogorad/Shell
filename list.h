#ifndef LIST_H
#define LIST_H

typedef struct list {
    char * str;
    struct list * next;
} list;

void init(list ** head);
void freemem(list * headlist);
list * addtolist(list * head, char * str, int lenbuff);
char * getName(int key, list * headlist);
int listsize(list * headlist);

#endif
