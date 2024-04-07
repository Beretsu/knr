#include <stdio.h>
#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void)		/* get a (possibly pushed-back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

int ungetch(int c)	/* push character back on input; return EOF on error */
{
	if (bufp >= BUFSIZE) {
		printf("ungetch: too many characters\n");
		return EOF;
	}
	return (c == EOF) ? EOF : (buf[bufp++] = c);
}
