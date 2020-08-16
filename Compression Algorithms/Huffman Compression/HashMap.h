#ifndef HASHMAP_H
#define HASHMAP_H

typedef struct {
	long long int* data;
	int capacity;
	int size;
} HashMap;

HashMap* allocHashMap(int);

void freeHashMap(HashMap*);

void incrementValue(HashMap*, int);

#endif