#include <stdio.h>
#include <stdlib.h>
#include "HashMap.h"
#include "HuffmanTree.h"
#include "BitWriter.h"
#include "BitReader.h"

#define MAX_CHAR 256	//in fact is 256 but we take it 260
#define NUMBER_OF_BITS_IN_A_BYTE 8

void readUncompressedDataAndComputeFrequencies(char* inputFileName, HashMap* frequencyCounter)
{
	FILE* input = fopen(inputFileName, "rb");

	unsigned char byteData;

	while (1)
	{
		fread(&byteData, 1, 1, input);

		if (feof(input))
		{
			break;
		}

		incrementValue(frequencyCounter, byteData);
	}

	fclose(input);

	return;
}

char** computeCodings(HuffmanTree* huffmanTree)
{
	char** codings = (char**)malloc(MAX_CHAR * sizeof(char*));

	int i;

	if (codings == NULL)
	{
		fprintf(stderr, "Error with malloc from main function!\n");

		freeHuffmanTree(huffmanTree);

		return NULL;
	}

	for (i = 0; i < MAX_CHAR; i++)
	{
		codings[i] = NULL;
	}

	writeCodings(huffmanTree, codings);

	return codings;
}

void visitHuffmanTreeToSaveItToFile(FILE* output, HuffmanTree* huffmanTree)
{
	if (huffmanTree->leftChild == NULL)	//it's a leaf node
	{
		writeBit(output, "1", 0);

		char string[NUMBER_OF_BITS_IN_A_BYTE + 1];
		int i;

		for (i = 0; i < NUMBER_OF_BITS_IN_A_BYTE; i++)
		{
			if (((huffmanTree->data) & (1 << (NUMBER_OF_BITS_IN_A_BYTE - 1 - i))) == 0)
			{
				string[i] = '0';
			}
			else
			{
				string[i] = '1';
			}
		}

		string[i] = '\0';
		
		writeBit(output, string, 0);
	}
	else
	{
		writeBit(output, "0", 0);
		
		visitHuffmanTreeToSaveItToFile(output, huffmanTree->leftChild);
		visitHuffmanTreeToSaveItToFile(output, huffmanTree->rightChild);
	}

	return;
}

void writeCompressedFile(char* inputFileName, char* outputFileName, HuffmanTree* huffmanTree, char** codings)
{
	FILE* input = fopen(inputFileName, "rb");
	FILE* output = fopen(outputFileName, "wb");

	unsigned char byteData;
	
	//reserv the first byte to file which represents the number of bits for padding at end of file

	writeBit(output, NULL, 2);

	//write the HuffmanTree to file

	visitHuffmanTreeToSaveItToFile(output, huffmanTree);

	//compress the input

	while (1)
	{
		if (fread(&byteData, 1, 1, input) != 1)
		{
			break;
		}

		if (feof(input))
		{
			break;
		}
		
		writeBit(output, codings[byteData], 0);
	}

	writeBit(output, NULL, 1);

	fclose(input);
	fclose(output);

	return;
}

void freeCodings(char** codings)
{
	int i;

	for (i = 0; i < MAX_CHAR; i++)
	{
		if (codings[i] != NULL)
		{
			free(codings[i]);
		}
	}

	free(codings);

	return;
}

void compress(char* input, char* output)
{
	HashMap* frequencyCounter = allocHashMap(256);
	
	readUncompressedDataAndComputeFrequencies(input, frequencyCounter);

	if (frequencyCounter->size == 0)
	{
		//file is empty

		FILE* outputFile = fopen(output, "wb");

		fclose(outputFile);

		freeHashMap(frequencyCounter);

		return;
	}
	
	HuffmanTree* huffmanTree = computeHuffmanTree(frequencyCounter);

	char** codings = computeCodings(huffmanTree);

	freeHashMap(frequencyCounter);

	writeCompressedFile(input, output, huffmanTree, codings);

	freeCodings(codings);
	freeHuffmanTree(huffmanTree);

	return;
}

HuffmanTree* restoreHuffmanTreeRecursive(FILE* input)
{
	char* string = readBit(input, 1, 0);

	HuffmanTree* result = NULL;
	
	if (string[0] == '1')
	{
		free(string);

		string = readBit(input, NUMBER_OF_BITS_IN_A_BYTE, 0);

		byte data = 0;
		int i;

		for (i = 0; i < NUMBER_OF_BITS_IN_A_BYTE; i++)
		{
			data = data << 1;
			data += string[i] - '0';
		}

		result = allocHuffmanTree(data, NULL, NULL, NULL);
	}
	else
	{
		HuffmanTree* leftChild = restoreHuffmanTreeRecursive(input);
		HuffmanTree* rightChild = restoreHuffmanTreeRecursive(input);

		result = allocHuffmanTree(0, leftChild, rightChild, NULL);
	}

	free(string);

	return result;
}

int visitHuffmanTreeToDecompress(FILE* input, FILE* output, HuffmanTree* huffmanTree)
{
	if (huffmanTree->leftChild == NULL)
	{
		char string[NUMBER_OF_BITS_IN_A_BYTE + 1];
		byte data = huffmanTree->data;

		int i = NUMBER_OF_BITS_IN_A_BYTE;
		
		string[i] = '\0';

		for (i = i - 1; i >= 0; i--)
		{
			string[i] = '0' + (data & 1);

			data = (data >> 1);
		}
		
		writeBit(output, string, 0);

		return 1;
	}
	else
	{
		char* string = readBit(input, 1, 0);

		if (string == NULL)
		{
			return 0;
		}

		if (string[0] == '0')
		{
			if (visitHuffmanTreeToDecompress(input, output, huffmanTree->leftChild) == 0)
			{
				free(string);

				return 0;
			}
		}
		else if(string[0] == '1')
		{
			if (visitHuffmanTreeToDecompress(input, output, huffmanTree->rightChild) == 0)
			{
				free(string);

				return 0;
			}
		}
		else
		{
			free(string);

			return 0;
		}

		free(string);

		return 1;
	}
}

void writeDecompressedFile(FILE* input, FILE* output, HuffmanTree* huffmanTree)
{
	unsigned char byteData;

	//decompress the input recursively

	if (huffmanTree->leftChild == NULL)
	{
		//only one node in the Huffman tree

		char string[NUMBER_OF_BITS_IN_A_BYTE + 1];
		byte data = huffmanTree->data;

		int i = NUMBER_OF_BITS_IN_A_BYTE;

		string[i] = '\0';

		for (i = i - 1; i >= 0; i--)
		{
			string[i] = '0' + (data & 1);

			data = (data >> 1);
		}

		char* buffer = readBit(input, 1, 0);

		while (buffer != NULL)
		{
			writeBit(output, string, 0);

			free(buffer);

			buffer = readBit(input, 1, 0);
		}

		return;
	}
	
	while (visitHuffmanTreeToDecompress(input, output, huffmanTree) == 1);

	return;
}

void decompress(char* inputFileName, char* outputFileName)
{
	FILE* input = fopen(inputFileName, "rb");

	fseek(input, 0, SEEK_END);	// seek to end of file
	int size = ftell(input);	// get current file pointer
	fseek(input, 0, SEEK_SET);	// seek back to beginning of file

	if (size == 0)
	{
		FILE* output = fopen(outputFileName, "wb");

		fclose(input);
		fclose(output);

		return;
	}

	readBit(input, 0, 1);
	
	HuffmanTree* huffmanTree = restoreHuffmanTreeRecursive(input);

	FILE* output = fopen(outputFileName, "wb");

	writeDecompressedFile(input, output, huffmanTree);

	fclose(input);
	fclose(output);
	freeHuffmanTree(huffmanTree);

	return;
}