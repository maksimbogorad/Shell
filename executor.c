#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "executor.h"
#include "list.h"

int changeIO(list * head, int * inOut, int ** fds, int ind, int numPrcs)
{
    int fd;
    if(inOut[0] && ind == 0)
    {
        fd = open(getName(inOut[0], head), O_RDWR | O_CREAT);
        if(fd != -1)
            dup2(fd, 0);
        else{
            perror("Can't open file");
            return 1;
        }
    }
    if(inOut[2] && ind == (numPrcs - 1))
    {
        fd = open(getName(inOut[2], head),O_RDWR|O_APPEND|O_CREAT,0777);
        if(fd != -1)
            dup2(fd, 1);
        else
        {
            perror("Can't create file");
            return 1;
        }
    }
    if(inOut[2] == 0 && inOut[1] && ind == (numPrcs - 1))
    {
        fd = open(getName(inOut[1], head), O_RDWR | O_CREAT, 0777);
        if(fd != -1)
            dup2(fd, 1);
        else
        {
            perror("Can't open file");
            return 1;
        }
    }
    if(ind != 0)
        dup2(fds[ind - 1][0], 0);
    if(ind != numPrcs - 1)
        dup2(fds[ind][1], 1);
    close(fds[ind][0]);

    return 0;
} 

int inIO(int key, int * inOut)
{
    int i;
    int inFlag = 0;
    for(i = 0; i < 3; i++)
        if(inOut[i] == key)
        {
            inFlag = 1;
            break;
        }
    return inFlag;
}

int countSizeIO(int * inOut)
{
    int res = 0, i = 0;
    for (i = 0; i < 3; i++)
        if(inOut[i] != 0)
            res++;
    if(inOut[2] > 0)
        res--;
    return res;
}

int getNumPrcs(char ** arr, int sizeArr)
{
    int cnt = 0;
    int i;
    for(i = 0; i < sizeArr; i++)
        if(arr[i] == NULL)
            cnt++;
    return cnt;
}

int getNextCmd(char ** arr, int prevInd)
{
    int i;
    for(i = prevInd; arr[i] != NULL; i++);
    return (i + 1);
}

char ** makearr(list * headlist, int size, int * inOut)
{
    list * tmp = headlist;
    int i;
    int sizeIO = countSizeIO(inOut);
    char ** arr = (char**)malloc((size - sizeIO + 1) * sizeof(*arr));
    for(i = size - 1; i >= 0; i--, tmp = tmp->next)
    {
        if(!inIO(i + 1, inOut))
        {
            if(tmp -> str != NULL)
            {
                arr[i] = (char*)malloc(strlen(tmp->str) + 1);
                strncpy(arr[i], tmp->str, strlen(tmp->str));
                arr[i][strlen(tmp->str)] = 0;
            }
            else
                arr[i] = NULL;
        }
    }
    arr[size - sizeIO] = NULL;
    return arr;
}

int sizeForCD(char ** arr)
{
    int i = 0;
    while(arr[i] != NULL)
        i++;
    return i;
}

void changedir(char ** arr, int size)
{
    int status;
    if(size == 2)
    {
        status = chdir(arr[1]);
        if(status == -1)
            perror(arr[1]);
    }
    else if(size == 1)
        printf("Too few args\n");
    else
        printf("Too many args\n");
}

void freeArrs(char ** arr, int ** fd, int size, int numPrcs)
{
    int k;
    for(k = 0; k < numPrcs; k++)
        free(fd[k]);
    for(k = 0; k < size; k++)
    {
        if(arr[k] != NULL)
            free(arr[k]);
    }
    free(arr);
    free(fd);
}

int ** makefds(int numPrcs)
{
    int k;
    int ** fd = (int**)malloc(sizeof(*fd) * numPrcs);
    for(k = 0; k < numPrcs; k++)
        fd[k] = (int*)malloc(2 * sizeof(int));
    return fd;
}

void execute(list * headlist, int mode, int * inOut)
{
    int k, pid, nextCmd = 0;
    int sizeIO = countSizeIO(inOut);
    int size = listsize(headlist);
    char ** arr = makearr(headlist, size, inOut);
    int numPrcs = getNumPrcs(arr, size - sizeIO + 1);
    int ** fd = makefds(numPrcs);
    for(k = 0; k < numPrcs; k++)
    {
        pipe(fd[k]);
        if(!strcmp(arr[nextCmd], "cd"))
            changedir(&arr[nextCmd], sizeForCD(&arr[nextCmd]));
        else{
            pid = fork();
            if(pid == 0){
                if(!changeIO(headlist, inOut, fd, k, numPrcs)){
                    execvp(arr[nextCmd], &arr[nextCmd]);
                    perror(arr[nextCmd]);
                }
                exit(1);
            }
            close(fd[k][1]);
            if(k != 0)
                close(fd[k - 1][0]);
            if(mode == 0)
                while(wait(NULL) != pid);
        }
        nextCmd = getNextCmd(arr, nextCmd);
    }
    freeArrs(arr, fd, size - sizeIO, numPrcs);
}
