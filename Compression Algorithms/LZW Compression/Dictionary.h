#ifndef DICTIONARY_H
#define DICTIONARY_H

#define INVALID_INDEX -1

typedef unsigned char byte;

typedef struct {
	int indexToPrefix;
	byte ending;

	int indexToFirst;
	int indexToLeft;
	int indexToRight;
} DictionaryEntry;

typedef struct {
	int capacity;
	int index;						//represents the index of the last valid element from the dictionary (in elements array)
	int bitSize;
	DictionaryEntry* elements;
} Dictionary;

DictionaryEntry* allocDictionaryEntry();

Dictionary* allocDictionary(int);

void initializeDictionary(Dictionary*);

void freeDictionaryEntry(DictionaryEntry*);

void freeDictionary(Dictionary*);

int searchAndAddToDictionaryIfNotFound(Dictionary*, DictionaryEntry*);

void showDictionary(Dictionary*);

#endif