#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

void init(list ** head)
{
    *head = NULL;
    printf(">> ");
}

void freemem(list * headlist)
{
    if (headlist != NULL) 
    {
        if (headlist -> str != NULL)
            free(headlist -> str);
        list * tmp = headlist;
        headlist = headlist -> next;
        free(tmp);
        freemem(headlist);
    }
}

list * addtolist(list * head, char * str, int lenbuff)
{
    list * tmp = (list*)malloc(sizeof(* tmp));
    if(lenbuff != -1)
    {
        tmp -> str = (char*)malloc(lenbuff + 1);
        strncpy(tmp -> str, str, lenbuff);
        tmp -> str[lenbuff] = 0;
    }
    else
        tmp -> str = NULL;
    tmp -> next = head;
    return tmp;
}

char * getName(int key, list * headlist)
{
    int it = 1;
    list * tpointer = headlist;
    while(tpointer != NULL)
    {
        if(key == it)
            return tpointer->str;
        it++;
        tpointer = tpointer->next;
    }
    return NULL;
}

int listsize(list * headlist)
{
    int iterator = 0;
    list * tmp = headlist;
    while(tmp != NULL){
        iterator++;
        tmp = tmp->next;
    }
    return iterator;
}
