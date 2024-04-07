/*
 * Exercise 1-22. Write a program to "fold" long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
*/

#include <stdio.h>
#define FSTOP 10				/* max columns in a line */
#define TABSIZE 8				/* size of tab */
#define MAXTAB FSTOP/TABSIZE*TABSIZE		/* last tabstop */
#define WORDBOUND 1				/* fold at last space */

int ntabstp(int);

int main()
{
	int c, i, j, ncol, fx, fc;
	char s[FSTOP];

	ncol = 0;		/* columns read so far */
	c = getchar();
	while (c != EOF) {
		fx = FSTOP;	/* folding index for current line */
		fc = 0;
		for (i=0; ncol<FSTOP && c!=EOF && c!='\n'	/* put FSTOP */
		&& (c!='\t' || ntabstp(ncol)<=MAXTAB); ++i) {	/* cols in s */
			if (i >= FSTOP) {
				for (j = 0; j < fx; j++)
					putchar(s[j]);
				if (fx != FSTOP) {
					putchar('\n');
					ncol = ncol - fc;
					for (j = fx; j < FSTOP; ++j)
						putchar(s[j]);
				}
				i = fc = 0;
				fx = FSTOP;
			}
			if (c == '\t') {
				fc = ncol = ntabstp(ncol);
				fx = i+1;
			} else if (c == '\b') {
				if (ncol > 0)
					--ncol;
			} else {
				if (c == ' ') {
					fx = i+1;
					fc = ++ncol;
				} else {
					++ncol;
				}
			}
			s[i] = c;
			c = getchar();
		}
		if (c=='\n' || c==EOF || fc==0 || WORDBOUND==0) { /* one */
			fx = i;
			ncol = 0;
		} else {			/* two lines */
			ncol = ncol-fc;
		}
		for (j = 0; j < fx; ++j)	/* print 1st line */
			putchar(s[j]);
		if (c != EOF)
			putchar('\n');
		for ( ; j < i; ++j)		/* print 2nd line if any */
			putchar(s[j]);
		if (c == '\n')
			c = getchar();
	}
	return 0;
}

/* ntabstp: return tabstop for col */
int ntabstp(int col)
{
	return col/TABSIZE * TABSIZE + TABSIZE;
}
