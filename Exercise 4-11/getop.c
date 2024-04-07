/*
 * Exercise 4-11. Modify getop so that it doesn't need to use ungetch.
 * Hint: use an internal static variable.
*/

#include <stdio.h>
#include <ctype.h>
#include "calc.h"
#define EMPTY 0
#define FULL 1

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int i;
	static int c, cstate = EMPTY;

	c = (cstate == EMPTY) ? getchar() : c;
	cstate = EMPTY;
	while (c == ' ' || c == '\t')
		c = getchar();
	s[0] = c;
	s[1] = '\0';
	if (!isdigit(c) && c != '.')
		return c;	/* not a number */
	i = 0;
	if (isdigit(c))		/* collect integer part */
		while (isdigit(s[++i] = c = getchar()))
			;
	if (c == '.')		/* collect fraction part */
		while (isdigit(s[++i] = c = getchar()))
			;
	s[i] = '\0';
	cstate = FULL;
	return NUMBER;
}
