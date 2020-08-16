#include <stdio.h>
#include <stdlib.h>
#include "BitReader.h"

#define NUMBER_OF_BITS_PER_READING 8

char* readBit(FILE* input, int numberOfBits, int mode)
{
	static char buffer = 0;
	static char pre_buffer = 0;
	static char counter = 0;
	static char padding = 0;
	static char endOfFile = 0;

	if (mode == 1)
	{
		fread(&padding, sizeof(char), 1, input);

		fread(&pre_buffer, sizeof(char), 1, input);

		return NULL;
	}
	
	char* string = (char*)malloc((numberOfBits + 1) * sizeof(char));
	
	if (string == NULL)
	{
		fprintf(stderr, "Error with malloc from readBit function!\n");

		return NULL;
	}
	   
	int i = 0;

	while (i < numberOfBits && i < counter)
	{
		if ((buffer & (1 << (NUMBER_OF_BITS_PER_READING - 1))) != 0)
		{
			string[i] = '1';
		}
		else
		{
			string[i] = '0';
		}

		buffer = buffer << 1;

		i++;
	}

	numberOfBits -= i;
	counter -= i;

	while (numberOfBits > 0)
	{
		if (endOfFile)
		{
			free(string);

			return NULL;
		}

		buffer = pre_buffer;
		counter = NUMBER_OF_BITS_PER_READING;

		if (fread(&pre_buffer, sizeof(char), 1, input) != 1)
		{
			counter -= padding;

			endOfFile = 1;
		}

		int j = 0;

		while (j < numberOfBits && j < counter)
		{
			if ((buffer & (1 << (NUMBER_OF_BITS_PER_READING - 1))) != 0)
			{
				string[i] = '1';
			}
			else
			{
				string[i] = '0';
			}

			buffer = buffer << 1;

			i++;
			j++;
		}

		numberOfBits -= j;
		counter -= j;
	}

	string[i] = '\0';
		
	return string;
}