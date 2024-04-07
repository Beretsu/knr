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
	int c, i, j, fx, ax, ncol, fcol;
	char s[FSTOP];

	ax = FSTOP;		/* index after last non-white char */
	ncol = 0;		/* columns read so far */
	c = getchar();
	while (c != EOF) {
		fx = FSTOP;	/* folding index for current line */
		fcol = 0;	/* columns read past folding index */
		for (i=0; ncol<FSTOP && c!=EOF && c!='\n'	/* put FSTOP */
		&& (c!='\t' || ntabstp(ncol)<=MAXTAB); ++i) {	/* cols in s */
			if (c == '\t') {
				if (ax == i) {		/* fold point */
					fx = ax;
					fcol = TABSIZE;
				} else
					fcol = ntabstp(fcol);
				ncol = ntabstp(ncol);
			} else if (c == ' ') {
				if (ax == i) {		/* fold point */
					fx = ax;
					fcol = 1;
				} else
					++fcol;
				++ncol;
			} else {
				ax = i+1;
				++fcol;
				++ncol;
			}
			s[i] = c;
			c = getchar();
		}
		if (WORDBOUND==0 || c=='\t' || c==' ')	/* fold at ax */
			fx = ax;
		if (c=='\n' || c==EOF || i<=fx) {	/* only one line */
			for (j = 0; j < i; ++j)
				putchar(s[j]);
			if (c != EOF)
				putchar('\n');
			if (c == '\n')
				c = getchar();
			ax = FSTOP;
			ncol = 0;
		} else {
			for (j = 0; j < fx; ++j)	/* 1st line */
				putchar(s[j]);
			putchar('\n');
			for ( ; j < i; ++j)		/* 2nd line */
				putchar(s[j]);
			if (fcol < FSTOP) {		/* continue 2nd line */
				if (ax == i)
					ax = 0;
				else
					ax = FSTOP;
				ncol = fcol;
			} else {			/* end 2nd line */
				putchar('\n');
				ax = FSTOP;
				ncol = 0;
			}
		}
	}
	return 0;
}

/* ntabstp: return tabstop for col */
int ntabstp(int col)
{
	return col/TABSIZE * TABSIZE + TABSIZE;
}
