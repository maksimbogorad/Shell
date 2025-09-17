#ifndef PARSER_H
#define PARSER_H

#include "list.h"

void handleIOquote(int lastc, int c, int * inOut, list * headlist, int i);
int isIOsymbol(int c);
int spaceTab(int c);
int spaceTabIOquote(int c);
int divide(int c);
int isWrongIO(list * headlist, int * inOut);
int isWrongPipe(list * head);
void processinglast(int * iterator, char * buff, list ** headlist);
void iscorrectcmd(int * flags, int chr, list ** head, int * inOut);

#endif
