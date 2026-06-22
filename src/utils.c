/* Additional functions */

#include "../include/common.h"
#include "../include/utils.h"

size_t lenstr(const char *s)
{
	const char * ptr = s;
	for (; *ptr != '\0'; ptr++);
	return ptr-s;
}

void stringCpy(char * dst, const char * src, size_t size)
{
	size_t i = 0;
	for (; src[i] != '\0' && i < size; i++)
		dst[i] = src[i];

	for (; i < size; i++)
		dst[i] = '\0';
}

void addSize(char ** string, size_t cur_len, uint16_t * newSize)
{
	char * newString = (char*)malloc(sizeof(char) * (*newSize)*2 + 1);
	if (newString == NULL)
	{
		perror("New string malloc error!\n");
		return;
	}
	stringCpy(newString, *string, cur_len+1);
	free(*string);
	*string = newString;
	*newSize *= 2;
}
