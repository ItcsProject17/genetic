/* file:	main.c													*/
/* authors:	Jurrian de Boer (j.h.d.de.boer@student.rug.nl			*/
/* 			Max Verbeek (email moet je zelf even toevoegen)			*/
/* date:	11-10-17												*/
/* version:	1.0														*/
/* description:														*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if 1 /*0 for assertions on, 1 for assertions off*/
	#define NDEBUG
#endif
#include <assert.h>

#define NUMBLOCKS 5
// number of blocks in the input set
#define CHROMLENGTH NUMBLOCKS
//length of chromosome
#define POPSIZE 10;
//number of chromosomes in population


/* Memory allocating function which indicates it if it fails.*/
void *safeMalloc(int n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("Error: malloc(%d) failed. Out of memory?\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Expects a pointer to an array and an integer n containing the amount of blocks. 
 * Scans for n integers indicating the sizes of each block. Stores the blocksizes
 * at the location of the inputted pointer */
void readBlocks(int **blocks, int numBlocks){
	*blocks = safeMalloc(numBlocks * sizeof(int));
	
	for (int i=0; i<numBlocks; i++){
		scanf("%d", (*blocks+i));
	}
	
}

/* Expects an integer length and returns a pointer to an array with that length
 * filled with random booleans. */
int *createChromosome(int length){
	int *chromosome;
	chromosome = safeMalloc(length*sizeof(int));
	
	srand(time(NULL)); // random seed
	for (int i=0; i<length; i++){
		chromosome[i] = ( rand() > (RAND_MAX / 2) ); // random boolean
	}
	
	return chromosome;
}

/* Debugging function */
void printArray(int *array, int length){
	printf("array:\n");
	for (int i=0; i<length; i++){
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	int *blocks;
	readBlocks(&blocks, NUMBLOCKS);
	
	int *testChrom;
	testChrom = createChromosome(CHROMLENGTH);
	
	// Print blocksizes and chromosome for later debugging
	printArray(blocks, NUMBLOCKS);
	printArray(testChrom, CHROMLENGTH);
	
	free(blocks);
	free(testChrom);
	
	return 0;
}
