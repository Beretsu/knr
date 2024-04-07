/*
 * Exercise 4-9. Our getch and ungetch do not handle a pushed-back EOF
 * correctly. Decide what their properties ought to be if an EOF is pushed back,
 * then implement your design.
*/

/*
 * Allowing for EOF would require buf to be declared as int instead of char if
 * we want portability, that takes much more space if BUFSIZE is large.
 * It could also cause trouble if EOF is pushed before it's actually reached.
 * Characters can be pushed back after EOF has reached, but this is useful
 * sometimes, Exercise 5-1 is an example.
 * EOF is a condition, not a value read in from the stream. Ungetting EOF
 * wouldn't change that condition (reached end of file).
*/

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
