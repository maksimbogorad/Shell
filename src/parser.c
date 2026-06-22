/* Parsing */

#include "../include/common.h"
#include "../include/parser.h"
#include "../include/list.h"
#include "../include/utils.h"

int checkSpecialSigns(List ** commands, char ** inf, char ** outf, int * back_mode, int * appmode)
{
	if ((*commands) == NULL)
		return NULL_TOKEN;
	else if (!strcmp((*commands)->part, "|") && (*commands)->in_quotes == 0)
		return PIPE_TOKEN;
	else if (!strcmp((*commands)->part, ">") && (*commands)->in_quotes == 0)
	{
		if ((*commands)->next == NULL)
			return NO_TOKEN;
		*outf = (*commands)->next->part;
		(*commands) = (*commands)->next;
		return OUT_REDIRECT_TOKEN;
	}
	else if (!strcmp((*commands)->part, "<") && (*commands)->in_quotes == 0)
	{
		if ((*commands)->next == NULL)
			return NO_TOKEN;
		*inf = (*commands)->next->part;
		(*commands) = (*commands)->next;
		return INPUT_REDIRECT_TOKEN;
	}
	else if (!strcmp((*commands)->part, ">>") && (*commands)->in_quotes == 0)
	{
		if ((*commands)->next == NULL)
			return NO_TOKEN;
		*outf = (*commands)->next->part;
		(*commands) = (*commands)->next;
		*appmode = ON;
		return ADD_REDIRECT_TOKEN;
	}
	else if (!strcmp((*commands)->part, "&") && (*commands)->in_quotes == 0)
	{
		*back_mode = 1;
		return BACKGROUND_TOKEN;
	}
	return NO_TOKEN;
}

int getSpecialToken(char c) 
{	
    switch(c) 
    {
        case '>': 
        	return OUT_REDIRECT_TOKEN;
        case '<': 
        	return INPUT_REDIRECT_TOKEN;
        case '|': 
        	return PIPE_TOKEN;
        case '&': 
        	return BACKGROUND_TOKEN;
        case '(': 
        	return BRACKET_OPEN_TOKEN;
        case ')': 
        	return BRACKET_CLOSE_TOKEN;
        default: 
        	return 0; 
    }
    return 0;
}

Commands * createCommandsList(List * commands, uint16_t listLen, int * commandsListLength, int * back_mode)
{
	int index = 0, appmode = OFF, i = 0;
	char * inf = NULL;
	char * outf = NULL;
	Commands * commandsList = NULL;
	char ** comms = (char**)malloc(sizeof(char*) * listLen+1);
	if (comms == NULL)
	{
		perror("Error comms malloc\n");
		return NULL;
	}

	while (i <= listLen)
	{
		int signCheckRes = checkSpecialSigns(&commands, &inf, &outf, back_mode, &appmode);
		if (signCheckRes == PIPE_TOKEN || commands == NULL || signCheckRes == NULL_TOKEN)
		{
			if (index > 0)
			{
				comms[index] = NULL;
				addCommand(&commandsList, comms, inf, outf, appmode, index);
				*commandsListLength+=1;
				appmode = OFF;
				comms = NULL;
				index = 0;
				inf = NULL;
				outf = NULL;
				if (commands != NULL)
				{
					comms = (char**)malloc(sizeof(char*) * listLen+1);
					if (comms == NULL)
					{
						perror("Error comms malloc\n");
						return NULL;
					}
				}
			}
			else
				free(comms);
		}
		else if (signCheckRes == NO_TOKEN)
		{
			comms[index] = (char*)malloc(sizeof(char) * lenstr(commands->part)+1);
			if (comms[index] == NULL)
			{
				for (uint16_t j = 0; j < index; j++) 
				{
	            	free(comms[j]);
	        	}
	        	free(comms);
				perror("Error comms element malloc\n");
				return NULL;
			}

			stringCpy(comms[index], commands->part, lenstr(commands->part)+1);
			index++;
		}
		if (commands != NULL)
			commands = commands->next;
		i++;
	}

	return commandsList;
}

void checkValidToken(int c, int token, uint16_t * amount_of_commands, int * is_in_quotes, List ** list)
{
	if (token)
	{
		int nextC = getchar();
		if (getSpecialToken(nextC) == token)
		{
			char tokenStr[3] = {c, nextC, 0};
			addElem(list, tokenStr, 3, *is_in_quotes);
			(*amount_of_commands)++;
			*is_in_quotes = 0;
		}
		else
		{
			char tokenStr[2] = {c, 0};
			addElem(list, tokenStr, 2, *is_in_quotes);
			(*amount_of_commands)++;
			*is_in_quotes = 0;
			ungetc(nextC, stdin);
		}
	}
}

void checkString(char ** string, List ** list, uint16_t * index, int * is_in_quotes, uint16_t * amount_of_commands, uint16_t * stringSize, int symbFlag)
{
	if (*string != NULL && symbFlag == normal)
	{
		(*string)[*index] = '\0';
		addElem(list, *string, *index, *is_in_quotes);
		free(*string);
		*string = NULL;
		(*amount_of_commands)++;
		*index = 0;
		*is_in_quotes = 0;
		*stringSize = BUF_SIZE;
	}
}
