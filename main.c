/* file:	main.c													*/
/* authors:	Jurrian de Boer     j.h.d.de.boer@student.rug.nl		*/
/* 			Max Verbeek         m.j.verbeek.2@student.rug.nl		*/
/* date:	11-10-17												*/
/* version:	1.0														*/
/* description:														*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include <assert.h>

// number of blocks in the input set
#define NUMBLOCKS 5

//length of chromosome
#define CHROMLENGTH NUMBLOCKS

//number of chromosomes in population
#define POPSIZE 10

// The array of blocks
int *blocks;

/* Memory allocating function which indicates it if it fails.*/
void *safeMalloc(int n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("Error: malloc(%d) failed. Out of memory?\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Expects a pointer to an array. 
 * Scans for n integers indicating the sizes of each block. Stores the blocksizes
 * at the location of the inputted pointer */
void readBlocks(int **array) {
	*array = safeMalloc(NUMBLOCKS * sizeof(int));
	
	for (int i = 0; i < NUMBLOCKS; i++) {
		scanf("%d", (*array + i));
	}
	
}

/* Returns a pointer to an array filled with random booleans. */
bool *createChromosome() {
	bool *chromosome;
	chromosome = safeMalloc(CHROMLENGTH * sizeof(int));
	
	for (int i=0; i < CHROMLENGTH; i++) {
		chromosome[i] = ( rand() > (RAND_MAX / 2) ); // random boolean
	}
	
	return chromosome;
}

/* Debugging function */
void printArray(int *array, int length) {
	printf("array:\n");
	for (int i=0; i<length; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

/* Prints the contents of a boolean array */
void printBoolArray(bool *array, int length) {
	printf("boolean array:\n");
	for (bool *b = array; b < array + length; b++) {
		printf("%d ", *b ? 1 : 0);
	}
	putchar('\n');
}

/* Mutates 1 gene in a chromosome */
void mutate(bool *chromosome) {
	int index = rand() % CHROMLENGTH;

	// flip the bit at a random index
	chromosome[index] = ! chromosome[index];
}

void crossover(bool *chrom1, bool *chrom2) {
	bool temp;

	// pick a random index that is between the start and the end
	int index = 1 + (rand() % (CHROMLENGTH - 1));

	#ifndef NDEBUG
		printf("[crossover] index chosen: %d\n", index);
		printf("[crossover] before 1:\n");
		printBoolArray(chrom1, CHROMLENGTH);
		printf("[crossover] before 2:\n");
		printBoolArray(chrom2, CHROMLENGTH);
	#endif

	// swap genes at all indeces before this random index
	for (int i = 0; i < index; i++) {
		temp = chrom1[i];
		chrom1[i] = chrom2[i];
		chrom2[i] = temp;
	}

	#ifndef NDEBUG
		printf("-----------------\n");
		printf("[crossover] after 1:\n");
		printBoolArray(chrom1, CHROMLENGTH);
		printf("[crossover] after 2:\n");
		printBoolArray(chrom2, CHROMLENGTH);
	#endif
}

int heightOfTower(bool *chromosome, bool b) {
	int sum = 0;

	// add the value behind the index of blocks, if the value behind
	// the index in chromosome == b
	for (int i = 0; i < CHROMLENGTH; i++) {
		if (chromosome[i] == b) sum += blocks[i];
	}

	return sum;
}

int main(int argc, char *argv[]) {
	readBlocks(&blocks);

	srand(time(NULL));
	
	bool *testChrom1, *testChrom2;
	testChrom1 = createChromosome(CHROMLENGTH);
	testChrom2 = createChromosome(CHROMLENGTH);

	crossover(testChrom1, testChrom2);
	
	#ifndef NDEBUG
		// Print blocksizes and chromosome for later debugging
		printf("\n\nArray contents:\n---------------\n");
		printArray(blocks, NUMBLOCKS);
		printBoolArray(testChrom1, CHROMLENGTH);
		printBoolArray(testChrom2, CHROMLENGTH);
	#endif

	free(blocks);
	free(testChrom1);
	free(testChrom2);

	return 0;
}
