#include "SpecialSym.h"

void strcopy(char* destination, char source[])
{
	size_t i = 0;
	for (; i < strlen(source); i++)
	{
		destination[i] = source[i];
	}

	destination[i] = '\0';

}
