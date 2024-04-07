/*
 * Exercise 1-13. Write a program to print a histogram of the lengths of words
 * in its input. It is easy to draw the histogram with the bars horizontal; a
 * vertical orientation is more challenging.
*/

#include <stdio.h>

#define MAXLEN 11		/* number of bars */
#define IN 1			/* inside a word */
#define OUT 0			/* outside a word */

/* print histogram with vertical bars */
int main()
{
	int c, len, state, maxlen, maxcount, i, j;
	int bar[MAXLEN];		/* count of lengths */

	len = 0;			/* length of word */
	maxlen = 0;			/* greatest length found so far */
	maxcount = 0;			/* greatest count of lengths */
	state = OUT;
	for (i = 0; i < MAXLEN; ++i)
		bar[i] = 0;
	
	/* check if white space didn't precede EOF, to not skip last word */
	while ((c = getchar()) != EOF || state == IN) {
		if (c == ' ' || c == '\t' || c == '\n' || c == EOF) {
			if (state == IN) {
				if (len >= MAXLEN) {
					if (++bar[MAXLEN-1] > maxcount)
						++maxcount;
				} else {
					if (++bar[len-1] > maxcount)
						++maxcount;
				}
				if (len > maxlen)
					maxlen = len;
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
	for (i = maxcount; i > 0; --i) {	/* top part */
		printf("%4d", i);
		for (j = 0; j < MAXLEN && j < maxlen; ++j)
			if (bar[j] >= i)
				printf("  #");
			else
				printf("   ");
		putchar('\n');
	}
	printf("    ");				/* space bottom row */
	for (i=0; i<MAXLEN-1 && i<maxlen; ++i)	/* bottom row */
		printf("%3d", (i+1));
	if (maxlen >= MAXLEN)			/* bottom right corner */
		printf(" >%2d", MAXLEN-1);
	putchar('\n');
}
