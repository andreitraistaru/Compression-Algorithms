#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LZWCompression.h"
#include "Dictionary.h"
#include "BitReader.h"
#include "BitWriter.h"

#define INITIAL_BITSIZE 8
#define MAXIMUM_BITSIZE 20
#define NUMBER_OF_BITS_BYTE 8
#define MAX 263

void recoverStringFromDictionary(Dictionary*, int, char**, unsigned int*, byte*);
int convertStringToInt(char*);
char* convertByteToString(byte, int);

void showDictionary(Dictionary* dictionary)
{
	char* string = (char*)malloc(1000 * sizeof(char));
	
	if (string == NULL)
	{
		return;
	}

	byte firstbyte;
	unsigned int stringCapacity;
	int i;
	for (i = 256; i <= dictionary->index; i++)
	{
		string[0] = '\0';
		stringCapacity = 1;
		recoverStringFromDictionary(dictionary, i, &string, &stringCapacity, &firstbyte);

		int j = 0, k = strlen(string);

		printf("i = %d : string: ", i);

		while (j < k)
		{
			char x[9];
			int y;
			for (y = 0; y < 8; y++)
			{
				x[y] = string[j + y];
			}
			x[y] = '\0';
			printf("%c", convertStringToInt(x));

			j += 8;;
		}

		printf("\n");
	}

	printf("-----------------------------------------------------------\n");

	free(string);

	return;
}

void compress(char* inputFileName, char* outputFileName)
{
	FILE* input = fopen(inputFileName, "rb");
	FILE* output = fopen(outputFileName, "wb");

	fseek(input, 0, SEEK_END);	// seek to end of file
	int size = ftell(input);	// get current file pointer
	fseek(input, 0, SEEK_SET);	// seek back to beginning of file

	if (size == 0)
	{
		fclose(input);
		fclose(output);

		return;
	}

	Dictionary* dictionary = allocDictionary(1 << MAXIMUM_BITSIZE);
	
	writeBit(output, NULL, 2);
	
	initializeDictionary(dictionary);

	DictionaryEntry* element = allocDictionaryEntry();
	element->indexToPrefix = INVALID_INDEX;

	int indexFound = INVALID_INDEX;
	char buffer[MAXIMUM_BITSIZE + 1];
	int i;

	while (fread(&(element->ending), sizeof(byte), 1, input) == 1)
	{
		if (dictionary->index + 1 == MAX)
		{
			for (i = 0; i < dictionary->bitSize; i++)
			{
				if ((element->indexToPrefix & (1 << (dictionary->bitSize - 1 - i))) == 0)
				{
					buffer[i] = '0';
				}
				else
				{
					buffer[i] = '1';
				}
			}

			buffer[i] = '\0';
			writeBit(output, buffer, 0);

			initializeDictionary(dictionary);
			
			element->indexToPrefix = INVALID_INDEX;
			indexFound = INVALID_INDEX;
		}

		indexFound = searchAndAddToDictionaryIfNotFound(dictionary, element);

		if (indexFound == INVALID_INDEX)
		{
			for (i = 0; i < dictionary->bitSize; i++)
			{
				if ((element->indexToPrefix & (1 << (dictionary->bitSize - 1 - i))) == 0)
				{
					buffer[i] = '0';
				}
				else
				{
					buffer[i] = '1';
				}
			}

			buffer[i] = '\0';
			writeBit(output, buffer, 0);

			element->indexToPrefix = element->ending;
		}
		else
		{
			element->indexToPrefix = indexFound;
		}
	}

	if (dictionary->bitSize == INITIAL_BITSIZE)
	{
		dictionary->bitSize++;
	}

	for (i = 0; i < dictionary->bitSize; i++)
	{
		if ((element->indexToPrefix & (1 << (dictionary->bitSize - 1 - i))) == 0)
		{
			buffer[i] = '0';
		}
		else
		{
			buffer[i] = '1';
		}
	}

	buffer[i] = '\0';

	writeBit(output, buffer, 0);
	writeBit(output, NULL, 1);

	freeDictionaryEntry(element);
	freeDictionary(dictionary);

	fclose(input);
	fclose(output);

	return;
}

char* convertByteToString(byte x, int numberOfBits)
{
	char* convertedInt = (char*)malloc((numberOfBits + 1) * sizeof(char));

	if (convertedInt == NULL)
	{
		fprintf(stderr, "Error with malloc from convertIntToString function!\n");

		return NULL;
	}

	int i;

	for (i = 0; i < numberOfBits; i++)
	{
		if ((x & (1 << (numberOfBits - 1 - i))) == 0)
		{
			convertedInt[i] = '0';
		}
		else
		{
			convertedInt[i] = '1';
		}
	}

	convertedInt[i] = '\0';

	return convertedInt;
}

int convertStringToInt(char* string)
{
	if (string == NULL)
	{
		return -1;
	}
	int x = 0, i = 0;

	while (string[i] != '\0')
	{
		x = x << 1;

		x += string[i] - '0';
	
		i++;
	}

	return x;
}

void addToString(char** string, unsigned int* stringCapacity, char* stringToAppend)
{
	if (*stringCapacity > strlen(stringToAppend) + strlen(*string))
	{
		strcat(*string, stringToAppend);
	
		return;
	}
		
	char* newString = (char*)realloc(*string, (strlen(stringToAppend) + strlen(*string) + 1) * sizeof(char));

	if (newString == NULL)
	{
		fprintf(stderr, "Error with malloc from addToString function!\n");

		return;
	}

	*string = newString;

	strcat(*string, stringToAppend);

	return;
}

void recoverStringFromDictionary(Dictionary* dictionary, int index, char** string, unsigned int* stringCapacity, byte* firstByte)
{
	if (dictionary->elements[index].indexToPrefix == INVALID_INDEX)
	{
		*firstByte = dictionary->elements[index].ending;

		char* stringToAppend = convertByteToString(*firstByte, NUMBER_OF_BITS_BYTE);

		addToString(string, stringCapacity, stringToAppend);

		free(stringToAppend);

		return;
	}

	recoverStringFromDictionary(dictionary, dictionary->elements[index].indexToPrefix, string, stringCapacity, firstByte);

	char* stringToAppend = convertByteToString(dictionary->elements[index].ending, NUMBER_OF_BITS_BYTE);

	addToString(string, stringCapacity, stringToAppend);

	free(stringToAppend);

	return;
}

void decompress(char* inputFileName, char* outputFileName)
{
	FILE* input = fopen(inputFileName, "rb");
	FILE* output = fopen(outputFileName, "wb");

	fseek(input, 0, SEEK_END);	// seek to end of file
	int size = ftell(input);	// get current file pointer
	fseek(input, 0, SEEK_SET);	// seek back to beginning of file

	if (size == 0)
	{
		fclose(input);
		fclose(output);

		return;
	}

	readBit(input, 0, 1);

	DictionaryEntry* element = allocDictionaryEntry();
	Dictionary* dictionary = allocDictionary(1 << MAXIMUM_BITSIZE);
	int oldIndex;
	unsigned int stringCapacity;
	byte firstByte;
	char* string = (char*)malloc((NUMBER_OF_BITS_BYTE + 1) * sizeof(char));
	char* buffer;
	
	if (string == NULL)
	{
		fprintf(stderr, "Error with malloc from decompress function!\n");

		fclose(input);
		fclose(output);

		freeDictionaryEntry(element);
		freeDictionary(dictionary);

		return;
	}

	initializeDictionary(dictionary);
	dictionary->bitSize++;
	oldIndex = INVALID_INDEX;
	stringCapacity = 0;
	string[0] = '\0';
	buffer = readBit(input, dictionary->bitSize, 0);

	element->indexToPrefix = convertStringToInt(buffer);
	
	free(buffer);

	buffer = convertByteToString(dictionary->elements[element->indexToPrefix].ending, NUMBER_OF_BITS_BYTE);

	writeBit(output, buffer, 0);

	free(buffer);

	oldIndex = element->indexToPrefix;
	
	while (1)
	{
		if (dictionary->index + 1 == MAX)
		{
			initializeDictionary(dictionary);
			dictionary->bitSize++;
			oldIndex = INVALID_INDEX;
			stringCapacity = 0;
			string[0] = '\0';
			buffer = readBit(input, dictionary->bitSize, 0);

			if (buffer == NULL)
			{
				free(string);

				fclose(input);
				fclose(output);

				freeDictionaryEntry(element);
				freeDictionary(dictionary);

				return;
			}

			element->indexToPrefix = convertStringToInt(buffer);

			free(buffer);

			buffer = convertByteToString(dictionary->elements[element->indexToPrefix].ending, NUMBER_OF_BITS_BYTE);
		
			writeBit(output, buffer, 0);

			free(buffer);

			oldIndex = element->indexToPrefix;
		}

		buffer = readBit(input, dictionary->bitSize, 0);

		if (buffer == NULL)
		{
			break;
		}

		element->indexToPrefix = convertStringToInt(buffer);

		free(buffer);

		string[0] = '\0';
		
		if (element->indexToPrefix <= dictionary->index)
		{
			recoverStringFromDictionary(dictionary, element->indexToPrefix, &string, &stringCapacity, &firstByte);

			writeBit(output, string, 0);
			
			dictionary->index++;

			if (dictionary->index == (1 << dictionary->bitSize) - 2)
			{
				dictionary->bitSize++;
			}
			
			dictionary->elements[dictionary->index].ending = firstByte;
			dictionary->elements[dictionary->index].indexToPrefix = oldIndex;
		}
		else
		{
			recoverStringFromDictionary(dictionary, oldIndex, &string, &stringCapacity, &firstByte);

			dictionary->index++;

			if (dictionary->index == (1 << dictionary->bitSize) - 2)
			{
				dictionary->bitSize++;
			}

			dictionary->elements[dictionary->index].ending = firstByte;
			dictionary->elements[dictionary->index].indexToPrefix = oldIndex;

			buffer = convertByteToString(firstByte, NUMBER_OF_BITS_BYTE);

			addToString(&string, &stringCapacity, buffer);

			free(buffer);

			writeBit(output, string, 0);
		}

		oldIndex = element->indexToPrefix;
	}

	free(string);

	fclose(input);
	fclose(output);

	freeDictionaryEntry(element);
	freeDictionary(dictionary);

	return;
}