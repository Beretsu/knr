/*
 * Exercise 1-22. Write a program to "fold" long input lines into two or more
 * shorter lines after the last non-blank character that occurs before the n-th
 * column of input. Make sure your program does something intelligent with very
 * long lines, and if there are no blanks or tabs before the specified column.
*/

#include <stdio.h>
#define FSTOP 80				/* max columns in a line */
#define TABSIZE 8				/* size of tab */
#define MAXTAB FSTOP/TABSIZE*TABSIZE		/* last tabstop */
#define WORDBOUND 1				/* fold at last space */

int ntabstp(int);

int main()
{
	int c, i, j, ncol, fx;
	char s[FSTOP];

	ncol = 0;		/* columns read so far */
	c = getchar();
	while (c != EOF) {
		fx = FSTOP;	/* folding index for current line */
		for (i=0; ncol<FSTOP && c!=EOF && c!='\n'	/* put FSTOP */
		&& (c!='\t' || ntabstp(ncol)<=MAXTAB); ++i) {	/* cols in s */
			if (c == '\t') {
				ncol = ntabstp(ncol);
				fx = i+1;
			} else {
				if (c == ' ')
					fx = i+1;
				++ncol;
			}
			s[i] = c;
			c = getchar();
		}
		if (c=='\n' || c==EOF || i<fx || WORDBOUND==0) { /* one line */
			fx = i;
			ncol = 0;
		} else {			/* two lines */
			ncol = i-fx;
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
