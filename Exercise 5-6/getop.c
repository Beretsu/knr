/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

#include <stdio.h>
#include <ctype.h>
#include "calc.h"

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int c;

	while ((*s = c = getch()) == ' ' || c == '\t')
		;
	*(s+1) = '\0';
	if (!isdigit(c) && c != '.')
		return c;	/* not a number */
	if (isdigit(c))		/* collect integer part */
		while (isdigit(*++s = c = getch()))
			;
	if (c == '.')		/* collect fraction part */
		while (isdigit(*++s = c = getch()))
			;
	*s = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}
