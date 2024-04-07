/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

#include <stdio.h>
#include <limits.h>

void reverse(char *s);

/* itoa: convert n to characters in s */
void itoa(int n, char *s)
{
	int sign;
	char *ps;

	if ((sign = n) < 0) {	/* record sign */
		if (n == INT_MIN)
			++n;
		n = -n;		/* make n positive */
	}
	ps = s;
	do {	/* generate digits in reverse order */
		*ps++ = n % 10 + '0';	/* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
	if (sign < 0) {
		*ps++ = '-';
		if (sign == INT_MIN)
			++*s;
	}
	*ps = '\0';
	reverse(s);
}
