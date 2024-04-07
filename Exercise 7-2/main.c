/*
 * Exercise 7-2. Write a program that will print arbitrary input in a sensible
 * way. As a minimum, it should print non-graphic characters in octal or
 * hexadecimal according to local custom, and break long text lines.
*/

#include <stdio.h>
#include <ctype.h>

#define LINEBREAK 16
#define TABSTOP 8
#define HEXFORM "%#.2x"
#define OCTFORM "%#.3o"
#define NONGRPHLEN 4

int main(int argc, char *argv[])
{
	int c, pos;
	char *f = HEXFORM;	/* string format */
	while (--argc > 0 && **++argv == '-')
		while ((c = *++*argv))
			f = (c == 'o') ? OCTFORM : (c == 'x') ? HEXFORM : f;

	while ((c = getchar()) != EOF) {
		if (isprint(c) || isspace(c)) {	/* graphic characters */
			if (c == '\t') { /* align tabs with line breaks */
				if (pos >= LINEBREAK - TABSTOP) {
					putchar('\n');
					pos = 0;
				}
				pos += TABSTOP - pos % TABSTOP;
			} else if (pos < LINEBREAK-1) {	/* continue on line */
				pos = (c == '\n') ? 0 : pos+1;
			} else if (c != '\n') {	/* break line */
				putchar('\n');
				pos = 1;
			} else {	/* naturally '\n' breaks line */
				pos = 0;
			}
			putchar(c);
		} else {	/* non-graphic characters */
			if (pos >= LINEBREAK-NONGRPHLEN-1) { /* break line */
				putchar('\n');
				pos = NONGRPHLEN;
			} else {	/* continue on line */
				pos += NONGRPHLEN;
			}
			printf(f, c);
		}
	}
	return 0;
}
