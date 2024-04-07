#include <stdio.h>
#include <string.h>
#define BUFSIZE 100

static char buf[BUFSIZE];	/* buffer for ungetch */
static int bufp = 0;		/* next free position in buf */

int getch(void)		/* get a (possibly pushed-back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

int ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: to many characters\n");
	else if (c == EOF)
		printf("ungetch: EOF not allowed\n");
	else
		return buf[bufp++] = c;
}

void ungets(char s[])
{
	int i;

	for (i = strlen(s)-1; i >= 0; --i)
		ungetch(s[i]);
}
