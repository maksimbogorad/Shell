/* Creating commands */

#include "../include/common.h"
#include "../include/commands.h"
#include "../include/utils.h"

int isEmptyCommandsList(Commands * node)
{
	return node == NULL;
}

void addCommand(Commands ** list, char ** commands, const char * in, const char * out, int app, uint16_t length)
{
	Commands * elem = (Commands*)malloc(sizeof(Commands));
	if (elem == NULL)
	{
		perror("Malloc error!\n");
		return;
	}
	elem->next = NULL;
	elem->comms = commands;
	elem->comms_length = length;
	if (in != NULL && in[0] != '\0')
	{
		elem->input_file = (char*)malloc(sizeof(char) * lenstr(in)+1);
		if (elem->input_file == NULL)
		{
			perror("Malloc error!\n");
			return;
		}
		stringCpy(elem->input_file, in, lenstr(in)+1);
	}
	else
	{
		elem->input_file = NULL;
	}
	if (out != NULL && out[0] != '\0')
	{
		elem->output_file = (char*)malloc(sizeof(char) * lenstr(out)+1);
		if (elem->output_file == NULL)
		{
			perror("Malloc error!\n");
			return;
		}
		stringCpy(elem->output_file, out, lenstr(out)+1);
	}
	else
	{
		elem->output_file = NULL;
	}
	elem->is_append = app;

	if (isEmptyCommandsList(*list))
	{
		*list = elem;
	}
	else
	{
		Commands * tmp = *list;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = elem;
	}
}

void freeCommandsList(Commands * list)
{
	if (!isEmptyCommandsList(list))
	{
		Commands * tmp1 = list;
		while (tmp1 != NULL)
		{
			Commands * tmp2 = tmp1;
			tmp1 = tmp1->next;
			for (int i = 0; i < tmp2->comms_length; i++)
				free(tmp2->comms[i]);
			free(tmp2->comms);
			free(tmp2);
		}
	}
}