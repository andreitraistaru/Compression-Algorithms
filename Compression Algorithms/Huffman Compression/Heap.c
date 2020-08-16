#include <stdio.h>
#include <stdlib.h>
#include "Heap.h"

int parent_index(int x)
{
	return (x - 1) / 2;
}

int left_child_index(int x)
{
	return (x * 2) + 1;
}

int right_child_index(int x)
{
	return (x * 2) + 2;
}

Heap* allocHeap(int capacity)
{
	Heap* newHeap = (Heap*)malloc(sizeof(Heap));

	if (newHeap == NULL)
	{
		fprintf(stderr, "Error with malloc from allocHeap function!\n");

		return NULL;
	}

	
	newHeap->elements = (HeapNode*)malloc(capacity * sizeof(HeapNode));

	if (newHeap->elements == NULL)
	{
		free(newHeap);

		fprintf(stderr, "Error with malloc from allocHeap function!\n");

		return NULL;
	}

	newHeap->capacity = capacity;
	newHeap->index = -1;

	return newHeap;
}

void freeHeap(Heap* heap)
{
	free(heap->elements);
	free(heap);

	return;
}

void siftUp(Heap* heap, int position)
{
	if (position == 0)
	{
		return;
	}

	if (heap->elements[position].priority < heap->elements[parent_index(position)].priority)
	{
		HeapNode aux = heap->elements[position];
		heap->elements[position] = heap->elements[parent_index(position)];
		heap->elements[parent_index(position)] = aux;

		siftUp(heap, parent_index(position));
	}

	return;
}

void siftDown(Heap* heap, int position)
{
	long long int priorityChild = -1;
	long long int indexChild = -1;

	if (left_child_index(position) <= heap->index)
	{
		priorityChild = heap->elements[left_child_index(position)].priority;
		indexChild = left_child_index(position);
	}

	if (right_child_index(position) <= heap->index)
	{
		if (priorityChild == -1 || priorityChild > heap->elements[right_child_index(position)].priority)
		{
			priorityChild = heap->elements[right_child_index(position)].priority;
			indexChild = right_child_index(position);
		}
	}

	if (indexChild != -1 && priorityChild < heap->elements[position].priority)
	{
		HeapNode aux = heap->elements[position];
		heap->elements[position] = heap->elements[indexChild];
		heap->elements[indexChild] = aux;
	
		siftDown(heap, indexChild);
	}

	return;
}

void insert(Heap* heap, HuffmanTree* key, long long int priority)
{
	heap->index++;

	heap->elements[heap->index].key = key;
	heap->elements[heap->index].priority = priority;
	
	siftUp(heap, heap->index);

	return;
}

void removeMin(Heap* heap, HeapNode* heapNodeRemoved)
{
	heapNodeRemoved->key = heap->elements[0].key;
	heapNodeRemoved->priority = heap->elements[0].priority;

	heap->elements[0] = heap->elements[heap->index];

	heap->index--;

	siftDown(heap, 0);

	return;
}

int size(Heap* heap)
{
	return heap->index + 1;
}