#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

typedef struct Commands
{
    char ** comms;
    int comms_length;
    char * input_file;
    char * output_file;
    int is_append;
    struct Commands * next;
} Commands;

int isEmptyCommandsList(Commands *node);
void addCommand(Commands **list, char **commands, const char *in, const char *out, int app, uint16_t length);
void freeCommandsList(Commands *list);

#endif
