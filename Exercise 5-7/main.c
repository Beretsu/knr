/*
 * Exercise 5-7. Rewrite readlines to store lines in an array supplied by main,
 * rather than calling alloc to maintain storage. How much faster is the
 * program?
*/

/*
 * Getting each line into a buffer and then copying into another is not
 * neccesary anymore; in fact, it never was, it was done that way to prepare
 * the reader for dynamic memmory allocation. But now that thats out of the
 * question, we can now get lines directly into allocbuf, this may not ideal
 * compared to using malloc and free, but its better than before. This also
 * avoids reading the last line if lineptr is full.
 *
 * ALLOCSIZE is now the limit for a line, as a consequence a single line can
 * fill up the buffer. readlines now also returns -1 on truncated line
 * encounter.
 *
 * limitations:
 * 	Assumes the last line ends with '\n'.
*/

#include <stdio.h>
#include <string.h>

#define MAXLINES 5000			/* max #lines to be sorted */
#define ALLOCSIZE 10000			/* storage for line of lines */

char *lineptr[MAXLINES];	/* pointers to text lines */

int readlines(char *lineptr[], char *allocbuf, int maxlines, int allocsize);
void writelines(char *lineptr[], int nlines);

void qsort(char *lineptr[], int left, int right);

/* sort input lines */
int main()
{
	int nlines;			/* number of input lines read */
	char allocbuf[ALLOCSIZE];	/* one big line of lines */

	if ((nlines=readlines(lineptr, allocbuf, MAXLINES, ALLOCSIZE))>=0) {
		qsort(lineptr, 0, nlines-1);
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("error: input to big to sort\n");
		return 1;
	}
}

int getline(char *, int);

/* readlines: read input lines */
int readlines(char *lineptr[], char *allocbuf, int maxlines, int allocsize)
{
	int len, nlines;

	nlines = 0;
	while (nlines < maxlines && allocsize > 1
	&& (len = getline(allocbuf, allocsize)) > 0) {
		allocbuf[len-1] = '\0';		/* delete newline */
		lineptr[nlines++] = allocbuf;
		allocbuf += len;
		allocsize -= len;
	}
	return (getchar() == EOF) ? nlines : -1;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

/* qsort: sort v[left]...v[right] into increasing order */
void qsort(char *v[], int left, int right)
{
	int i, last;
	void swap(char *v[], int i, int j);

	if (left >= right)	/* do nothing if array contains */
		return;		/* fewer than two elements */
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if (strcmp(v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	qsort(v, left, last-1);
	qsort(v, last+1, right);
}

/* swap: interchange v[i] and v[j] */
void swap(char *v[], int i, int j)
{
	char *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}

