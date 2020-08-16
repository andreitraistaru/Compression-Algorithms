#ifndef HEAP_H
#define HEAP_H

#include "HuffmanTree.h"

typedef struct {
	HuffmanTree* key;
	long long int priority;
} HeapNode;

typedef struct {
	int capacity;
	int index;
	HeapNode* elements;
} Heap;

Heap* allocHeap(int);

void freeHeap(Heap*);

void insert(Heap*, HuffmanTree*, long long int);

void removeMin(Heap*, HeapNode*);

int size(Heap*);

#endif