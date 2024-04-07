/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
*/

#include <stdio.h>

#define START ' '		/* start character */
#define END '~'			/* end character */
#define NBARS END-START+1	/* number of bars */

/* prints histogram horizontally. ascii only */
int main()
{
	int i, j, bar[NBARS];

	for (i = 0; i < NBARS; ++i)
		bar[i] = 0;
	while ((i = getchar()) != EOF)		/* count */
		if (i >= START && i <= END)
			++bar[i-START];

	putchar('\n');
	for (i = 0; i < NBARS; ++i) {		/* print table */
		printf(" %c ", i+START);
		for (j = 0; j < bar[i]; ++j)
			putchar('#');
		printf("[%d]\n", bar[i]);
	}
}
