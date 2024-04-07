/*
 * Exercise 4-8. Suppose that there will never be more than one character of
 * pushback. Modify getch and ungetch accordingly.
*/

#include <stdio.h>
#define FULL 1
#define EMPTY 0

char buf;		/* buffer for ungetch */
char bufstate;		/* signal if buf is empty or full */

int getch(void)         /* get a (possibly pushed-back) character */
{
	if (bufstate == FULL) {
		bufstate = EMPTY;
		return buf;
	} else {
		return getchar();
	}
}

void ungetch(int c)     /* push character back on input */
{
        if (bufstate == FULL) {
                printf("ungetch: buf is full\n");
	} else {
                buf = c;
		bufstate = FULL;
	}
}

