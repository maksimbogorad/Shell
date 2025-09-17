#ifndef UTILS_H
#define UTILS_H

#include "list.h"

char * extendbuff(char * buff, int * lenbuff);
void reinit(list ** headlist);
void killbg();
void freememory(list * headlist, char * buff);

#endif
