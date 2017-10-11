/* file:	skeleton.c												*/
/* author:	Jurrian (j.h.d.de.boer@student.rug.nl)					*/
/* date:	06-09-17												*/
/* version:	1.0														*/
/* description:														*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 /*0 for assertions on, 1 for assertions off*/
	#define NDEBUG
#endif
#include <assert.h>

#define NUMBLOCKS 5
// number of blocks in the input set
#define CHROMLENGTH numBlocks;
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


int main(int argc, char *argv[]) {
	int *blocks;
	readBlocks(&blocks, NUMBLOCKS);
	
	// Print blocksizes for later debugging
	printf("Blocks:\n");
	for (int i; i<NUMBLOCKS; i++){
		printf("%d ", blocks[i]);
	}
	printf("\n");
	
	return 0;
}
