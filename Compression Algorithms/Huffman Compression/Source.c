#include <stdio.h>
#include <string.h>
#include <time.h>
#include "HuffmanCompression.h"

int main(int argc, char* argv[])
{
	time_t begin = clock();

	if (strcmp(argv[1], "1") == 0)
	{
		compress(argv[2], argv[3]);
	}
	else if(strcmp(argv[1], "2") == 0)
	{
		decompress(argv[2], argv[3]);
	}
	
	clock_t end = clock();

	double time = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("Execution time: %f seconds\n", time);

	return 0;
}