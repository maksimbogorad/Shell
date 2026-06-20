/* Working with list */

#include "../include/common.h"
#include "../include/list.h"
#include "../include/utils.h"

void initList(List ** node)
{
	*node = NULL;
}

int isEmptyList(List * node)
{
	return node == NULL;
}

void addElem(List ** node, const char * str, uint16_t stringSize, int is_in_quotes)
{
	List * tmp = (List*)malloc(sizeof(List));
	if (tmp == NULL)
	{
		perror("List malloc error!\n");
		return;
	}
	tmp->next = NULL;
	tmp->in_quotes = is_in_quotes;
	tmp->part = (char*)malloc(sizeof(char) * stringSize + 1);
	if (tmp->part == NULL)
	{
		perror("String malloc error!\n");
		free(tmp);
		return;
	}
	stringCpy(tmp->part, str, stringSize+1);
	if (isEmptyList(*node))
	{
		*node = tmp;
	}
	else
	{
		List * tmp1 = *node;
		while (tmp1->next != NULL)
		{
			tmp1 = tmp1->next;
		}
		tmp1->next = tmp;
	}
}

void freeList(List * list)
{
	if (!isEmptyList(list))
	{
		List * tmp1 = list;
		while (tmp1 != NULL)
		{
			List * tmp2 = tmp1;
			tmp1 = tmp1->next;
			free(tmp2->part);
			free(tmp2);
		}
	}
}
