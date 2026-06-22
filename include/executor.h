#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "commands.h"
#include "list.h"

int redirectIO(const char * inputfile, const char * outputfile, int append_mode);
int checkSpecialcommands(List * commands, uint16_t listLen);
void execute(List *list, uint16_t listLen);
void executionAlone(Commands *commandsList, int back_mode);
void conveyorExecute(Commands *commandsList, int commandsListLength, uint16_t listLen, int back_mode);

#endif
