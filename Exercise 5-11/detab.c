/*
 * Exercise 5-11. Modify the program entab and detab (written as exercises in
 * Chapter 1) to accept a list of tab stops as arguments. Use the default tab
 * settings if there are no arguments.
*/

/*
 * This program does not try to achieve the same spacing like Exercise 1-20
 * This is only possible when tab size is the default.
 * Instead this program copies the UNIX program "expand".
*/

#include <stdio.h>
#define TABSIZE 8			/* default tab size */
#define MAXSTOPS 100			/* maximum input tab stops */
#define FALLBACKTABSIZE 1		/* tab size after the list ends */

int tabsizes[MAXSTOPS];		/* tab sizes from command line args */
int nstops;			/* total # of tab stops in the list */

int detab(int);
int parseargs(int, char **);

int main(int argc, char *argv[])
{
	int c, i, t;	/* t: current tab size */

	if (parseargs(argc, argv))
		return 1;
	t = *tabsizes;
	if (nstops > 1) {
		i = 1;
		while ((c = detab(t)) != EOF)
			if (c == '\n')
				t = tabsizes[(i=1)-1];
			else if (i < nstops)
				t = tabsizes[i++];
			else
				t = FALLBACKTABSIZE;
	} else {
		while (detab(t) != EOF)
			;
	}
	return 0;
}

/* parseargs: 0 if the list of tab sizes is valid; 1 otherwise */
int parseargs(int argc, char *argv[])
{
	int max, temp, atoi(char *);

	if (argc == 1) {
		*tabsizes = TABSIZE;	/* default to TABSIZE */
		return 0;
	}
	if (--argc > MAXSTOPS) {
		printf("error: to many arguments\n");
		return 1;
	}
	max = 0;
	while (argc-- > 0)
		if ((temp = atoi(*++argv)) <= max) {
			printf("error: tab stops must be ascending and > 0\n");
			return 1;
		} else {
			tabsizes[nstops++] = temp - max;
			max = temp;
		}
	return 0;
}

/* atoi: convert s to integer; only for positive values */
int atoi(char *s)
{
	int n;

	for (n = 0; *s >= '0' && *s <= '9'; s++)
		n = n * 10 + (*s - '0');
	return n;
}

/* detab: replaces tabs with blanks, up to tabsize; returns last char read */
int detab(int tabsize)
{
	int c;
	int pos;	/* position relative to TABSIZE */

	pos = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\t') {
			while (pos < tabsize) {
				putchar(' ');
				pos++;
			}
			return c;
		} else {
			putchar(c);
			if (c == '\n' || pos == tabsize-1)
				return c;
			else
				++pos;
		}
	}
	return c;
}
