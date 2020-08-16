#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "HashMap.h"

typedef unsigned char byte;

typedef struct node {
	byte data;
	struct node* leftChild;
	struct node* rightChild;
	struct node* parent;
} HuffmanTree;

HuffmanTree* allocHuffmanTree(byte, HuffmanTree*, HuffmanTree*, HuffmanTree*);

void freeHuffmanTree(HuffmanTree*);

void showHuffmanTree(HuffmanTree*);

HuffmanTree* computeHuffmanTree(HashMap*);

void writeCodings(HuffmanTree*, char**);

#endif