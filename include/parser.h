#ifndef PARSER_H
#define PARSER_H

#include "list.h"
#include "commands.h"

#define BUF_SIZE 8

typedef enum
{
	PIPE_TOKEN = 1,
	INPUT_REDIRECT_TOKEN,
	OUT_REDIRECT_TOKEN,
	ADD_REDIRECT_TOKEN,
	BRACKET_OPEN_TOKEN,
	BRACKET_CLOSE_TOKEN,
	SEPARATE_TOKEN,
	BACKGROUND_TOKEN,
	NO_TOKEN,
	NULL_TOKEN
} tokens;

enum
{
	normal,
	quoteFlag
};

enum
{
	contBlock,
	contOpen
};

enum
{
	OFF,
	ON
};

int getSpecialToken(char c);
int checkSpecialSigns(List **commands, char **inf, char **outf, int *back_mode, int *appmode);
Commands *createCommandsList(List *commands, uint16_t listLen, int *commandsListLength, int *back_mode);
void checkValidToken(int c, int token, uint16_t * amount_of_commands, int * is_in_quotes, List ** list);
void checkString(char ** string, List ** list, uint16_t * index, int * is_in_quotes, uint16_t * amount_of_commands, uint16_t * stringSize, int symbFlag);

#endif