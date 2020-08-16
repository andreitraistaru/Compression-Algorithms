#include <stdio.h>
#include <stdlib.h>
#include "HashMap.h"

HashMap* allocHashMap(int capacity)
{
	HashMap* newHashMap = (HashMap*)malloc(sizeof(HashMap));

	if (newHashMap == NULL)
	{
		fprintf(stderr, "Error with malloc from allocHashMap function!\n");

		return NULL;
	}

	newHashMap->data = (long long int*)malloc(capacity * sizeof(long long int));

	if (newHashMap->data == NULL)
	{
		free(newHashMap);

		fprintf(stderr, "Error with malloc from allocHashMap function!\n");

		return NULL;
	}

	newHashMap->capacity = capacity;
	int i;

	for (i = 0; i < capacity; i++)
	{
		newHashMap->data[i] = 0;
	}

	newHashMap->size = 0;

	return newHashMap;
}

void freeHashMap(HashMap* hashMap)
{
	free(hashMap->data);
	free(hashMap);

	return;
}

void incrementValue(HashMap* hashMap, int key)
{
	if (hashMap->data[key] == 0)
	{
		hashMap->size++;
	}

	hashMap->data[key]++;
}

void showHashMap(HashMap* hashMap)
{
	int i = 0;

	for (i = 0; i < hashMap->capacity; i++)
	{
		if (hashMap->data[i] != 0)
		{
			printf("Key:%c \t Value:%lld\n", i, hashMap->data[i]);
		}
	}
}