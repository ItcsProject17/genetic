/* file:    main.c                                                  */
/* authors: Jurrian de Boer     j.h.d.de.boer@student.rug.nl        */
/*          Max Verbeek         m.j.verbeek.2@student.rug.nl        */
/* date:    26-10-17                                                */
/* version: 1.0                                                     */
/* description:                                                     */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

#ifndef NDEBUG
	// No double negations for debug code
	#define DEBUG
#endif

// number of blocks in the input set
#define NUMBLOCKS 5

//length of chromosome
#define CHROMLENGTH NUMBLOCKS

//number of chromosomes in population
#define POPSIZE 10

// the 1 in x chance that mutation occurs for chromosomes.
#define MUTATION_RATE 20

// The array of blocks
int *blocks;

// The generation of chromosomes
bool **generation;

/* Memory allocating function which prints an error message if it fails. */
void *safeMalloc(size_t n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("Error: malloc(%ld) failed. Out of memory?\n", n);
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
		chromosome[i] = rand() > RAND_MAX / 2; // random boolean
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
		printf("%d ", *b);
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
	// pick a random index that is between the start and the end
	int index = 1 + (rand() % (CHROMLENGTH - 1));

	#ifdef DEBUG
		printf("[crossover] index chosen: %d\n", index);
		printf("[crossover] before 1:\n");
		printBoolArray(chrom1, CHROMLENGTH);
		printf("[crossover] before 2:\n");
		printBoolArray(chrom2, CHROMLENGTH);
	#endif

	// swap genes at all indeces before this random index
	for (int i = 0; i < index; i++) {
		bool temp = chrom1[i];
		chrom1[i] = chrom2[i];
		chrom2[i] = temp;
	}

	#ifdef DEBUG
		printf("-----------------\n");
		printf("[crossover] after 1:\n");
		printBoolArray(chrom1, CHROMLENGTH);
		printf("[crossover] after 2:\n");
		printBoolArray(chrom2, CHROMLENGTH);
	#endif
}

/**
 * This function returns the height of the tower of the T set of
 * chromosome if b == true, and that of the F set otherwise
 */
int heightOfTower(bool *chromosome, bool b) {
	int sum = 0;

	// add the value behind the index of blocks, if the value behind
	// the index in chromosome == b
	for (int i = 0; i < CHROMLENGTH; i++) {
		if (chromosome[i] == b) sum += blocks[i];
	}

	return sum;
}

/**
 * Returns the difference in height between two stacks formed by a chromosome
 */
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

/* This function assumes that the generation is sorted */
void changeGeneration(bool **generation) {
	// the first 2 chromosomes are our best ones. We keep those unchanged.
	// The last 2 chromosomes are not good. We replace them with
	// crossed-over versions of the first 2 chromosomes.
	generation[POPSIZE - 2] = generation[0];
	generation[POPSIZE - 1] = generation[1];
	crossover(generation[POPSIZE - 2], generation[POPSIZE - 1]);

	// as for every chromosome in between, there will be
	// a 1 in MUTATION_RATE chance for mutation
	for (int i = 2; i < POPSIZE - 2; i++) {
		if (rand() <= RAND_MAX / MUTATION_RATE) {
			mutate(generation[i]);
		}
	}
}

int compareChromosomes(const void *pa, const void *pb) {
	bool *chromA = (bool *) pa;
	bool *chromB = (bool *) pb;

	// return a negative number if a < b
	// return 0 if a == b
	// return a positive number if a > b
	return heightDifference(chromA) - heightDifference(chromB);
}

int main(int argc, char *argv[]) {
	readBlocks(&blocks);

	srand(time(NULL));

	// Generate a new generation. A generation consists of 10 chromosomes.
	// Each chromosome is represented by an amount of booleans.
	// A chromosome will have the same amount of booleans as there are blocks.
	generation = safeMalloc(POPSIZE * sizeof(bool *));
	for (int i = 0; i < POPSIZE; i++) {
		generation[i] = createChromosome(CHROMLENGTH);
	}

	bool bestChromosome[CHROMLENGTH];

	// nGen represents the generation number
	int nGen = 0, smallestDiff = 99999;

	while (smallestDiff > 10 && nGen < 200) {
		// sort our generation according to the distance in the blocks
		// this puts the most succesful chromosome at position 0
		qsort(generation, POPSIZE, sizeof(generation[0]), compareChromosomes);

		// save the best chromosome
		int smallestGenerationDifference = heightDifference(generation[0]);
		if (smallestGenerationDifference < smallestDiff) {
			memcpy(bestChromosome, generation[0], sizeof(bestChromosome));
			smallestDiff = smallestGenerationDifference;
		}

		nGen++;
	}
	
	printBoolArray(bestChromosome, CHROMLENGTH);

	// output our best result
	printf("Tower 1:\n");
	for (int i = 0; i < CHROMLENGTH; i++) {
		if (bestChromosome[i]) {
			printf("%d ", blocks[i]);
		}
	}
	printf("\n");

	printf("Tower 2:\n");
	for (int i = 0; i < CHROMLENGTH; i++) {
		if (! bestChromosome[i]) {
			printf("%d ", blocks[i]);
		}
	}
	printf("\n");

	// Memory cleanup
	free(blocks);
	for (int i = 0; i < POPSIZE; i++) {
		free(generation[i]);
	}
	free(generation);

	return 0;
}
