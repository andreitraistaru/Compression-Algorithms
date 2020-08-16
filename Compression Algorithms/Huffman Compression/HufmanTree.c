#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HuffmanTree.h"
#include "HashMap.h"
#include "Heap.h"

#define MAX_HEIGHT_OF_HUFFMAN_TREE 260	//in fact is 256 but we take it 260

HuffmanTree* allocHuffmanTree(byte data, HuffmanTree* leftChild, HuffmanTree* rightChild, HuffmanTree* parent)
{
	HuffmanTree* newHuffmanTree = (HuffmanTree*)malloc(sizeof(HuffmanTree));

	if (newHuffmanTree == NULL)
	{
		fprintf(stderr, "Error with malloc from allocHuffmanTree!\n");

		return NULL;
	}

	newHuffmanTree->data = data;
	newHuffmanTree->leftChild = leftChild;
	newHuffmanTree->rightChild = rightChild;
	newHuffmanTree->parent = parent;

	return newHuffmanTree;
}

void freeHuffmanTree(HuffmanTree* root)
{
	if (root == NULL)
	{
		return;
	}

	freeHuffmanTree(root->leftChild);
	freeHuffmanTree(root->rightChild);

	free(root);

	return;
}

HuffmanTree* computeHuffmanTree(HashMap* frequencyCounter)
{
	Heap* heap = allocHeap(frequencyCounter->size);

	int i = 0;

	for (i = 0; i < frequencyCounter->capacity; i++)
	{
		if (frequencyCounter->data[i] > 0)
		{
			HuffmanTree* newLeaf = allocHuffmanTree((byte) i, NULL, NULL, NULL);
			
			insert(heap, newLeaf, frequencyCounter->data[i]);
		}
	}

	HeapNode node1;
	HeapNode node2;

	while (size(heap) > 1)
	{
		removeMin(heap, &node1);
		removeMin(heap, &node2);

		HuffmanTree* newTree = allocHuffmanTree(node1.priority + node2.priority, node1.key, node2.key, NULL);

		node1.key->parent = newTree;
		node2.key->parent = newTree;
		
		insert(heap, newTree, node1.priority + node2.priority);
	}

	removeMin(heap, &node1);

	freeHeap(heap);

	return node1.key;
}

void findCodingRecursive(HuffmanTree* huffmanTree, char* currentCoding, int currentIndex, char** codings)
{
	if (huffmanTree->leftChild == NULL)	//a non leaf node must have both children not NULL
	{
		if (currentCoding[0] == '\0')
		{
			//only one node in the Huffman tree

			codings[huffmanTree->data] = (char*)malloc(2 * sizeof(char));

			if (codings[huffmanTree->data] == NULL)
			{
				fprintf(stderr, "Error with malloc from findCodingRecursive function!\n");

				return;
			}

			codings[huffmanTree->data][0] = '0';
			codings[huffmanTree->data][1] = '\0';

			return;
		}

		//so, it is a leaf; I have to write its coding

		codings[huffmanTree->data] = (char*)malloc((1 + currentIndex) * sizeof(char));

		if (codings[huffmanTree->data] == NULL)
		{
			fprintf(stderr, "Error with malloc from findCodingRecursive function!\n");

			return;
		}
		
		strcpy(codings[huffmanTree->data], currentCoding);

		return;
	}

	//it is not a leaf
	//go to the left child

	currentCoding[currentIndex] = '0';
	currentIndex++;
	currentCoding[currentIndex] = '\0';

	findCodingRecursive(huffmanTree->leftChild, currentCoding, currentIndex, codings);

	//go to the right child

	currentCoding[currentIndex - 1] = '1';

	findCodingRecursive(huffmanTree->rightChild, currentCoding, currentIndex, codings);

	//return from this recursion

	currentIndex--;
	currentCoding[currentIndex] = '\0';

	return;
}

void writeCodings(HuffmanTree* huffmanTree, char** codings)
{
	char* currentcoding = (char*)malloc(MAX_HEIGHT_OF_HUFFMAN_TREE * sizeof(char));

	if (currentcoding == NULL)
	{
		fprintf(stderr, "Error with malloc from writeCodings function!\n");

		return;
	}

	currentcoding[0] = '\0';

	findCodingRecursive(huffmanTree, currentcoding, 0, codings);

	free(currentcoding);
}
