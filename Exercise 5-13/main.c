/*
 * Exercise 5-13. Write the program tail, which prints the last n lines of its
 * input. By default, n is 10, let us say, but it can be changed by an
 * optional argument, so that
 * 	tail -n
 * prints the last n lines. The program should behave rationally no matter how
 * unreasonable the input or the value of n. Write the program so it makes the
 * best use of available storage; lines should be stored as in the sorting
 * program of Section 5.6, not in a two-dimensional array of fixed size.
*/

/* 
 * BUFSIZE = INT_MAX+2U to allow for the theoretical max n (MAX_INT), only
 * empty lines can fit in this case though. +2 for '\0's
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE INT_MAX+2U	/* max input */
#define TAILSIZE 10		/* default tail size */

char *readlines(char *buf, int n);
void writelines(char *buf, char *start);

/* print last n lines of its input */
int main(int argc, char *argv[])
{
	int c, n;			/* n: last n lines to print */
	char buf[BUFSIZE];		/* buffer for input lines */
	char *start;			/* pointer to first line in buf */

	if (argc>2 || argc==2 && **++argv!='-') {
		printf("usage: tail -n, where n > 0\n");
		return 1;
	}
	if ((n = (argc==1 || *++*argv=='\0') ? TAILSIZE : 0) == 0)
		while (c = *(*argv)++) {
			if (!isdigit(c)) {
				printf("error: argument	must be only digits\n");
				return 1;
			}
			if (n > INT_MAX/10 || (c-'0') > INT_MAX-(n*=10)) {
				printf("error: argument is to big\n");
				return 1;
			}
			n += (c-'0');
		}
	if ((start = readlines(buf, n)) == NULL) {
		printf("error: argument or input to big\n");
		return 1;
	} else if (n > 0)
		writelines(buf, start);
	return 0;
}

char **palloc(int n);
void pafree(char **p);

/* readlines: read into buf the last n lines of its input.
 * returns a pointer to the first of those lines; buf is a circular array */
char *readlines(char *buf, int n)
{
	int c, startln = 0;		/* startln: signal a new line */
	char *cursor, *buflim;		/* cursor: position to write to buf */
	char **lines;			/* pointers to lines; circular */
	char **firstln, **lastln;	/* lastln: position to write to lines */
	char **lineslim;

	if ((lines = palloc(n)) == 0)
		return NULL;
	*lines = cursor = buf;
	firstln = lastln = lines;
	lineslim = lines + n;		/* lines: circles at n */
	buflim = buf + BUFSIZE;		/* buf: circles at BUFSIZE */
	while ((c = getchar()) != EOF) {
		if (startln) {		/* starting a new line */
			if (++lastln == lineslim)
				lastln = firstln++;
			else if (lastln == firstln && ++firstln == lineslim)
				firstln = lines;
			*lastln = cursor;
		}
		if (cursor+1 == *firstln)
			return NULL;
		if (cursor+1 == buflim)
			if (*firstln == buf || *firstln == buf+1)
				return NULL;
			else
				cursor = buf;	/* circle buf around */
		startln = (*cursor++ = c) == '\n';
	}
	*cursor = *--buflim = '\0';		/* two pieces of lines */
	if (*firstln < cursor && *firstln > buf)
		*buf = '\0';			/* ignore initial piece */
	cursor = *firstln;	/* keep track of *firstln to return before */
	pafree(lines);		/* freeing lines */
	return cursor;
}

/* writelines: write lines in buf; buf is circular, starting at start */
void writelines(char *buf, char *start)
{
	printf("%s%s", start, (start != buf) ? buf : "\0");
}

