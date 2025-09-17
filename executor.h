#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "list.h"

int changeIO(list * head, int * inOut, int ** fds, int ind, int numPrcs);
int inIO(int key, int * inOut);
int countSizeIO(int * inOut);
int getNumPrcs(char ** arr, int sizeArr);
int getNextCmd(char ** arr, int prevInd);
char ** makearr(list * headlist, int size, int * inOut);
int sizeForCD(char ** arr);
void changedir(char ** arr, int size);
void freeArrs(char ** arr, int ** fd, int size, int numPrcs);
int ** makefds(int numPrcs);
void execute(list * headlist, int mode, int * inOut);

#endif
