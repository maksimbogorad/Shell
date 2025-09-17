#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "utils.h"
#include "list.h"

char * extendbuff(char * buff, int * lenbuff)
{
    char * newBuff = (char*)malloc((*lenbuff) * 2);
    strncpy(newBuff, buff, (*lenbuff) - 1);
    *lenbuff *= 2;
    free(buff);
    return newBuff;
}

void reinit(list ** headlist)
{
    freemem(*headlist);
    init(headlist);
}

void killbg()
{
    while(wait4(-1, NULL, WNOHANG, NULL) > 0);
}

void freememory(list * headlist, char * buff)
{
    freemem(headlist);
    free(buff);
    printf("\n");
}
