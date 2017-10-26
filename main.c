/* file:	main.c													*/
/* authors:	Jurrian de Boer     j.h.d.de.boer@student.rug.nl		*/
/* 			Max Verbeek         m.j.verbeek.2@student.rug.nl		*/
/* date:	26-10-17												*/
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

// The generation of chromosomes
bool **generation;

/* Memory allocating function which prints an error message if it fails. */
void *safeMalloc(int n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("Error: malloc(%d) failed. Out of memory?\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Expects a pointer to an array. Scans for NUMBLOCK integers indicating 
 * the sizes of each block. Stores the blocksizes at the location of the 
 * inputted pointer. */
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

/* This function returns the height of the tower of the T set of
 * chromosome if b == true, and that of the F set otherwise */
int heightOfTower(bool *chromosome, bool b) {
	int sum = 0;

	// add the value behind the index of blocks, if the value behind
	// the index in chromosome == b
	for (int i = 0; i < CHROMLENGTH; i++) {
		if (chromosome[i] == b) sum += blocks[i];
	}

	return sum;
}

/* Returns the difference in height between two stacks formed by a chromosome */
// Misschien heightOfTower en heightDifference samenvoegen??
int heightDifference(bool *chromosome) {
	int sum = 0;
	sum += heightOfTower(chromosome, 0);
	sum -= heightOfTower(chromosome, 1);
	return (sum >= 0 ? sum : -sum);
}

void getSmallestDifference(int *index, int *smallestDif) {
	int diff;
	for (int i = 0; i < POPSIZE; i++) {
		diff = heightDifference(generation[i]);
		if (diff < *smallestDif) {
			*smallestDif = diff;
			*index = i;
		}
	}
}

int main(int argc, char *argv[]) {
	readBlocks(&blocks);

	srand(time(NULL));
	
	generation = safeMalloc(POPSIZE * sizeof(bool *));
	for (int i = 0; i < POPSIZE; i++) {
		generation[i] = createChromosome(CHROMLENGTH);
	}
	
	#ifndef NDEBUG
		// Print blocksizes and chromosome for later debugging
		printf("\n\nArray contents:\n---------------\n");
		printArray(blocks, NUMBLOCKS);
		for (int i = 0; i < POPSIZE; i++) {
			printBoolArray(generation[i], CHROMLENGTH);
		}
		
		printf("\n\nCrossing over gene 0 and 1:\n");
		crossover(generation[0], generation[1]);
		for (int i = 0; i < POPSIZE; i++) {
			printBoolArray(generation[i], CHROMLENGTH);
		}
	#endif
	
	int nGen = 0, bestChromIndex, smallestDiff = 99999;
	while (smallestDiff > 10 && nGen < 20) {
		// Make new generation
		
		// Perform selection
		
		getSmallestDifference(&bestChromIndex, &smallestDiff);
		nGen++; // Keep track of the generation number.
	}
	
	printf("Generation: %d\n", nGen);
	
	// Memory cleanup
	free(blocks);
	for (int i = 0; i < POPSIZE; i++) {
		free(generation[i]);
	}
	free(generation);

	return 0;
}
