/* Main shell */

#include "../include/common.h"
#include "../include/shell.h"

void shell(void)
{
	initSignals();
	int c, symbFlag = normal, token = 0, is_in_quotes = 0;
	uint16_t stringSize = BUF_SIZE, index = 0, amount_of_commands = 0;
	char * string = NULL;
	List * list;
	initList(&list);

	fprintf(stdout, "> ");

	while ((c = getchar()) != EOF)
	{
		if (symbFlag != quoteFlag)
			token = getSpecialToken(c);
		else
			token = 0;
		if (c == '\n')
		{	
			if (symbFlag == quoteFlag)
				fprintf(stderr, "Error: unmatched quotes\n");
			checkString(&string, &list, &index, &is_in_quotes, &amount_of_commands, &stringSize, symbFlag);
			if (list != NULL)
			{
				if (symbFlag != quoteFlag)
					execute(list, amount_of_commands);
				freeList(list);
				amount_of_commands = 0;
				initList(&list);
			}
			checkZombie();
			fprintf(stdout, "> ");
			fflush(stdout);
			symbFlag = normal;
			continue;
		}
		if (c == ' ' || token)
		{
			checkString(&string, &list, &index, &is_in_quotes, &amount_of_commands, &stringSize, symbFlag);
			checkValidToken(c, token, &amount_of_commands, &is_in_quotes, &list);
			if (symbFlag != quoteFlag)
				continue;
		}
		if (c == '\"')
		{
			if (symbFlag == normal)
			{
				is_in_quotes = 1;
				symbFlag = quoteFlag;
			}
			else
			{
				symbFlag = normal;
			}
			continue;
		}
		if (string == NULL)
		{
			string = (char*)malloc(sizeof(char) * BUF_SIZE + 1);
			if (string == NULL)
			{
				perror("String malloc error!\n");
				return;
			}
		}
		if (index >= stringSize)
		{
			addSize(&string, (size_t)index, &stringSize);
		}
		if (!token)
		{
			string[index] = c;
			index++;
		}
	}
}
