#include <stdio.h>
#include <stdlib.h>
#include "Dictionary.h"

#define INITIAL_BITSIZE 8

DictionaryEntry* allocDictionaryEntry()
{
	DictionaryEntry* newDictionaryEntry = (DictionaryEntry*)malloc(sizeof(DictionaryEntry));

	if (newDictionaryEntry == NULL)
	{
		fprintf(stderr, "Error with malloc from allocDictionaryEntry function!\n");

		return NULL;
	}

	return newDictionaryEntry;
}

Dictionary* allocDictionary(int capacity)
{
	Dictionary* newDictionary = (Dictionary*)malloc(sizeof(Dictionary));

	if (newDictionary == NULL)
	{
		fprintf(stderr, "Error with malloc from allocDictionary function!\n");

		return NULL;
	}

	newDictionary->elements = (DictionaryEntry*)malloc(capacity * sizeof(DictionaryEntry));

	if (newDictionary->elements == NULL)
	{
		free(newDictionary);

		fprintf(stderr, "Error with malloc from allocDictionary function!\n");

		return NULL;
	}

	newDictionary->capacity = capacity;
	newDictionary->index = -1;

	return newDictionary;
}

void initializeDictionary(Dictionary* dictionary)
{
	int initialSizeOfDictionary = 1 << INITIAL_BITSIZE;

	if (dictionary->capacity < initialSizeOfDictionary)
	{
		fprintf(stderr, "You provided a dictionary without enought capacity as is necessary to be properly initialized!\n");

		return;
	}

	int i;

	for (i = 0; i < initialSizeOfDictionary; i++)
	{
		dictionary->elements[i].indexToPrefix = INVALID_INDEX;
		dictionary->elements[i].ending = i;
		dictionary->elements[i].indexToFirst = INVALID_INDEX;

		dictionary->elements[i].indexToLeft = INVALID_INDEX;
		dictionary->elements[i].indexToRight = INVALID_INDEX;

		/*if (i == 0)
		{
			dictionary->elements[i].indexToLeft = INVALID_INDEX;
		}
		else
		{
			dictionary->elements[i].indexToLeft = i - 1;
		}

		if (i == INITIAL_SIZE_OF_DICTIONARY - 1)
		{
			dictionary->elements[i].indexToRight = INVALID_INDEX;
		}
		else
		{
			dictionary->elements[i].indexToRight = i + 1;
		}*/
	}

	dictionary->index = initialSizeOfDictionary - 1;
	dictionary->bitSize = INITIAL_BITSIZE;

	return;
}

void freeDictionaryEntry(DictionaryEntry* dictionaryEntry)
{
	free(dictionaryEntry);
}

void freeDictionary(Dictionary* dictionary)
{
	free(dictionary->elements);
	free(dictionary);

	return;
}

void addDictionaryEntry(Dictionary* dictionary, DictionaryEntry* element, int previousIndex)
{
	dictionary->index++;

	if (dictionary->index == (1 << dictionary->bitSize))
	{
		dictionary->bitSize++;
	}

	dictionary->elements[dictionary->index] = *element;
	dictionary->elements[dictionary->index].indexToFirst = INVALID_INDEX;
	dictionary->elements[dictionary->index].indexToLeft = INVALID_INDEX;
	dictionary->elements[dictionary->index].indexToRight = INVALID_INDEX;
	
	if (previousIndex == INVALID_INDEX)
	{
		dictionary->elements[element->indexToPrefix].indexToFirst = dictionary->index;
	}
	else
	{
		if (dictionary->elements[previousIndex].ending < element->ending)
		{
			dictionary->elements[previousIndex].indexToRight = dictionary->index;
		}
		else
		{
			dictionary->elements[previousIndex].indexToLeft = dictionary->index;
		}
	}

	/*if (dictionary->elements[element->indexToPrefix].indexToFirst == INVALID_INDEX)
	{
		dictionary->elements[element->indexToPrefix].indexToFirst = dictionary->index;

		dictionary->elements[element->indexToPrefix].indexToLeft = INVALID_INDEX;
		dictionary->elements[element->indexToPrefix].indexToRight = INVALID_INDEX;
	}
	else
	{
		if (dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].ending > element->ending)
		{
			dictionary->elements[dictionary->index].indexToLeft = INVALID_INDEX;
			dictionary->elements[dictionary->index].indexToRight = dictionary->elements[element->indexToPrefix].indexToFirst;
			
			dictionary->elements[element->indexToPrefix].indexToFirst = dictionary->index;
		}
		else
		{
			dictionary->elements[dictionary->index].indexToLeft = dictionary->elements[element->indexToPrefix].indexToFirst;
			dictionary->elements[dictionary->index].indexToRight = INVALID_INDEX;
			dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].indexToRight = dictionary->index;

			dictionary->elements[element->indexToPrefix].indexToFirst = dictionary->index;
		}
	}

	
	

	if (dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].ending > element->ending)
	{
		dictionary->elements[dictionary->index].indexToLeft = INVALID_INDEX;
		dictionary->elements[dictionary->index].indexToRight = dictionary->elements[element->indexToPrefix].indexToFirst;

		dictionary->elements[dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].indexToLeft].indexToRight = dictionary->index;
		dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].indexToLeft = dictionary->index;
	}
	else
	{
		dictionary->elements[dictionary->index].indexToLeft = dictionary->elements[element->indexToPrefix].indexToFirst;
		dictionary->elements[dictionary->index].indexToRight = INVALID_INDEX;

		dictionary->elements[dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].indexToLeft].indexToRight = dictionary->index;
		dictionary->elements[dictionary->elements[element->indexToPrefix].indexToFirst].indexToLeft = dictionary->index;
	}*/
	
	return;
}

int searchAndAddToDictionaryIfNotFound(Dictionary* dictionary, DictionaryEntry* element)
{
	if (element->indexToPrefix == INVALID_INDEX)
	{
		return element->ending;
	}
	
	int currentIndex, previousIndex;

	previousIndex = INVALID_INDEX;
	currentIndex = dictionary->elements[element->indexToPrefix].indexToFirst;

	while (1)
	{
		if (currentIndex == INVALID_INDEX)
		{
			addDictionaryEntry(dictionary, element, previousIndex);

			return INVALID_INDEX;
		}

		if (dictionary->elements[currentIndex].ending == element->ending)
		{
			return currentIndex;
		}

		if (dictionary->elements[currentIndex].ending < element->ending)
		{
			previousIndex = currentIndex;
			currentIndex = dictionary->elements[currentIndex].indexToRight;
		}
		else
		{
			previousIndex = currentIndex;
			currentIndex = dictionary->elements[currentIndex].indexToLeft;
		}
	}
}