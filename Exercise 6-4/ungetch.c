#include <stdio.h>
#define FULL 1
#define EMPTY 0

static char buf;		/* buffer for ungetch */
static char bufstate;		/* signal if buf is empty or full */

int getch(void)         /* get a (possibly pushed-back) character */
{
	if (bufstate == FULL) {
		bufstate = EMPTY;
		return buf;
	} else {
		return getchar();
	}
}

int ungetch(int c)     /* push character back on input */
{
        if (bufstate == FULL) {
                printf("ungetch: buf is full\n");
		return EOF;
	} else if (c == EOF) {
		return EOF;
	} else {
		bufstate = FULL;
                return buf = c;
	}
}

