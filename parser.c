#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "list.h"
#include "executor.h"

void handleIOquote(int lastc, int c, int * inOut, list * headlist, int i)
{
    int size = listsize(headlist) + 1;
    if(i > 0)
        size++;
    if(c == '<')
        if(inOut[0] == 0)
            inOut[0] = size;
        else
            inOut[0] = -1;
    else if(c == '>' && lastc == '>')
        if(inOut[2] == 0)
            inOut[2] = size;
        else
            inOut[0] = -1;
    else if(c == '>')
        if(inOut[1] == 0)
            inOut[1] = size;
        else
            inOut[0] = -1;
    else
        inOut[0] = -1;
}

int isIOsymbol(int c)
{
    return ((c == '>') || (c == '<'));
}

int spaceTab(int c)
{
    return ((c == '\t') || (c == ' '));
}

int spaceTabIOquote(int c)
{
    return ((c == ' ') || (c == '\t') || (c == '<') || (c == '>'));
}

int divide(int c)
{
    return (c == ' ') || (c == '\t') || (c == '&') || (c == '>') || (c == '<') || (c == '|');
}

int isWrongIO(list * headlist, int * inOut)
{
    int size = listsize(headlist);
    int i = 0;
    if(inOut[0] && (inOut[0] == inOut[1] || inOut[0] == inOut[2]))
        return 1;
    for(i = 0; i < 3; i++)
        if(inOut[i] > size)
            return 1;
    if(inOut[0] == -1)
        return 1;
    return 0;
}

int isWrongPipe(list * head)
{
    int errflag = 0;
    int pared = 0;
    list * tmp = head;
    if(tmp != NULL && tmp -> str == NULL)
        errflag = 1;
    while(tmp != NULL)
    {
        if(tmp -> str == NULL && pared == 1)
            errflag = 1;
        if(tmp -> str == NULL)
            pared = 1;
        else 
            pared = 0;
        tmp = tmp -> next;
    }
    tmp = head;
    if(tmp != NULL)
    {
        while(tmp -> next != NULL)
            tmp = tmp -> next;
        if(tmp -> str == NULL)
            errflag = 1;
    }
    return errflag;
}

void processinglast(int * iterator, char * buff, list ** headlist)
{
    if(*iterator != 0)
        *headlist = addtolist(*headlist, buff, *iterator);
    *iterator = 0;
}

void iscorrectcmd(int * flags, int chr, list ** head, int * inOut)
{
    int i;
    int mode = (flags[1] == 1);
    if (flags[0] || (flags[1] > 1) 
        || (flags[1] == 1 && chr != '&') 
        || isWrongIO(*head, inOut) || isWrongPipe(*head))
        printf("incorrect input\n");
    else if((*head) != NULL)
        execute(*head, mode, inOut);
    flags[0] = 0;
    flags[1] = 0;
    for(i = 0; i < 3; i++)
        inOut[i] = 0;
}
