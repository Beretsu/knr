/*
 * Exercise 5-12. Extend entab and detab to accept the shorthand
 *      entab -m +n
 * to mean tab stops every n columns, starting at column m. Choose convenient
 * (for the user) default behavior.
*/

/*
 * if m is not set, then it defaults to TABSIZE.
 * if n is not set, then it defaults to m.
*/

#include <stdio.h>
#include <ctype.h>

#define TABSIZE 8		/* default tab size */
#define NOTSET -1

int tabsize = TABSIZE;		/* the n from +n */
int startpos = NOTSET;		/* the m from -m */

int detab(int);
int parseargs(int, char **);

int main(int argc, char *argv[])
{
	int c, t;	/* t: current tab size */

	if (parseargs(argc, argv))
		return 1;
	t = startpos;
	while ((c = detab(t)) != EOF)
		t = (c == '\n') ? startpos : tabsize;
	return 0;
}

/* parseargs: 0 if args are valid; 1 otherwise */
int parseargs(int argc, char *argv[])
{
        int c, atoi(char *);
        char *prog = *argv;             /* name of the program */

        if (argc > 3) {
                printf("%s: to many arguments\n", prog);
                return 1;
        }
        while (--argc>0 && ((c=**++argv)=='+' || c=='-') && isdigit(*++*argv))
                if (c == '-')
                        startpos = atoi(*argv);
                else
                        tabsize = atoi(*argv);
        if (argc!=0 || tabsize==0 || startpos==0) {
        	printf("usage: %s -m +n, m > 0 and n > 0\n", prog);
                return 1;
	}
	startpos = (startpos == NOTSET) ? tabsize : startpos;
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
