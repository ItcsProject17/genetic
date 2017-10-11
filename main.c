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

/* Memory allocating function which indicates it if it fails.*/
void *safeMalloc(int n) {
    void *p = malloc(n);
    if (p == NULL) {
        printf("Error: malloc(%d) failed. Out of memory?\n", n);
        exit(EXIT_FAILURE);
    }
    return p;
}

/* Expects an integer n determining the amount of blocks followed by n integers
 * indicating the sizes of each block. Returns the pointer to an array
 * containing all the block heights.*/
int *readBlocks(){
	int numBlocks;
	scanf("%d", &numBlocks);
	int *blocks;
	blocks = safeMalloc(numBlocks * sizeof(int));
	
	for (int i=0; i<numBlocks; i++){
		scanf("%d", (numBlocks+i));
	}
	
	return blocks;
}


int main(int argc, char *argv[]) {
	int *blocks;
	blocks = readBlocks();
}
