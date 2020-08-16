#include <stdio.h>
#include <string.h>
#include "BitWriter.h"

#define NUMBER_OF_BITS_PER_WRITING 8

void writeBit(FILE* output, char* input, char mode)
{
	static char buffer = 0;
	static char counter = 0;

	int i, length;

	if (mode == 1)
	{
		//equivalent to flush the buffer (add padding if necessary)
		
		char padding = 8 - counter;

		if (counter > 0)
		{
			buffer = buffer << padding;

			fwrite(&buffer, sizeof(char), 1, output);

			counter = 0;
			buffer = 0;
		}
		else
		{
			padding = 0;
		}

		fseek(output, 0, SEEK_SET);

		fwrite(&padding, sizeof(char), 1, output);

		fseek(output, 0, SEEK_END);

		return;
	}
	else if (mode == 2)
	{
		fseek(output, 1, SEEK_SET);

		return;
	}

	//normal writing
	
	length = strlen(input);

	for (i = 0; i < length; i++)
	{
		buffer = buffer << 1;
		buffer += input[i] - '0';

		counter++;

		if (counter == NUMBER_OF_BITS_PER_WRITING)
		{
			fwrite(&buffer, sizeof(char), 1, output);

			counter = 0;
			buffer = 0;
		}
	}

	return;
}