/*
 * Exercise 1-13. Write a program to print a histogram of the lengths of words
 * in its input. It is easy to draw the histogram with the bars horizontal; a
 * vertical orientation is more challenging.
*/

#include <stdio.h>

#define MAXLEN 11		/* number of bars */
#define IN 1			/* inside a word */
#define OUT 0			/* outside a word */

/* print histogram with horizontal bars */
int main()
{
	int c, len, state, max, i, j;
	int bar[MAXLEN];	/* count of lengths */

	len = 0;		/* length of word */
	max = 0;		/* greatest length found so far */
	state = OUT;
	for (i = 0; i < MAXLEN; ++i)
		bar[i] = 0;

	/* check if white space didn't precede EOF, to not skip last word */
	while ((c = getchar()) != EOF || state == IN) {
		if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
			if (state == IN) {
				if (len >= MAXLEN)
					++bar[MAXLEN-1];
				else
					++bar[len-1];
				if (len > max)
					max = len;
				len = 0;
				state = OUT;
			}
		} else {
			++len;
			state = IN;
		}
	}

	putchar('\n');
	/* print table up to the last non-empty bar */
	for (i = 0; i < MAXLEN-1 && i < max; ++i) {	/* all bars but last */
		printf("%3d ", i+1);
		for (j = 0; j < bar[i]; ++j)
			putchar('#');
		printf("[%d]\n", bar[i]);
	}
	if (max >= MAXLEN) {				/* last bar */
		printf(">%2d ", MAXLEN-1);
		for (j = 0; j < bar[MAXLEN-1]; ++j)
			putchar('#');
		printf("[%d]\n", bar[MAXLEN-1]);
	}
}
