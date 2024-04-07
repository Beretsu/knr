/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
*/

#include <stdio.h>

#define MAXCHAR 256		/* maximum chars in a set */
#define ASCII_A 65		/* ascii value for 'A' */
#define ASCII_START 32		/* ascii start of graphical chars */
#define ASCII_END 127		/* ascii end of graphical chars (excluding) */
#define EBCDIC_A 193		/* ebcdic value for 'A' */
#define EBCDIC_START 64		/* ebcdic start of graphical chars */
#define EBCDIC_END 250		/* ebcdic end of graphical chars (excluding) */


/* prints histogram vertically. Good for ascii and ebcdic (untested); for other
 * sets it prints even non-graphical characters, causing misalignment */
int main()
{
	int i, j, maxval, start, end;
	int bar[MAXCHAR];	/* stores the frequencies */

	if ('A' == ASCII_A) {			/* char set is ascii */
		start = ASCII_START;
		end = ASCII_END;
	} else if ('A' == EBCDIC_A) {		/* char set is ebcdic */
		start = EBCDIC_START;
		end = EBCDIC_END;
	} else {				/* unknown char set */
		start = 0;
		end = 0;
	}
	maxval = 0;		/* maximum value stored in bar */
	for (i = 0; i < MAXCHAR; ++i)
		bar[i] = 0;
	while ((i = getchar()) != EOF)	/* store count of chars */
		if (i >= start && i < end || i == '\n' || i == '\t')
			if (++bar[i] > maxval)
				maxval = bar[i];

	putchar('\n');
	for (i = maxval; i > 0; --i) {	/* print top part of the table */
		printf("%4d ", i);
		for (j = 0; j < MAXCHAR; ++j) {
			if (j >= start && j < end || j=='\n' || j=='\t') {
				if (bar[j] >= i)
					putchar('#');
				else
					putchar(' ');
			}
		}
		putchar('\n');
	}
	printf("     ");			/* space 1st char row */
	for (i = 0; i < MAXCHAR; ++i) {		/* print it */
		if (i == '\n' || i == '\t')	/* for \n and \t */
			putchar('\\');		/* n and t goes in 2nd row */
		else if (i >= start && i < end)
			putchar(i);
	}
	printf("\n     ");			/* space 2nd char row */
	j = 2;					/* for n and t */
	for (i = 0; i < MAXCHAR && j > 0; ++i) {	/* print it */
		if (i == '\n') {
			putchar('n');
			--j;
		} else if (i == '\t') {
			putchar('t');
			--j;
		} else if (i >= start && i < end)
			putchar(' ');
	}
	putchar('\n');
}
