/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
*/

#include <stdio.h>

#define START ' '		/* start character */
#define END '~'			/* end character */
#define NBARS END-START+1	/* number of bars */

/* prints histogram vertically. ascii only */
int main()
{
	int i, j, max, bar[NBARS];

	max = 0;			/* max count */
	for (i = 0; i < NBARS; ++i)
		bar[i] = 0;
	while ((i = getchar()) != EOF)	/* count */
		if (i >= START && i <= END)
			if (++bar[i-START] > max)
				max = bar[i-START];

	putchar('\n');
	for (i = max; i > 0; --i) {	/* print top part */
		printf("%3d ", i);
		for (j = 0; j < NBARS; ++j) {
			if (bar[j] >= i)
				putchar('#');
			else
				putchar(' ');
		}
		putchar('\n');
	}
	printf("    ");			/* space bottom row */
	for (i = 0; i < NBARS; ++i)	/* print it */
		putchar(i+START);
	putchar('\n');
}
