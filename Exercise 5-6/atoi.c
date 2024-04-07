/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

#include <ctype.h>

/* atoi: convert s to integer */
int atoi(char *s)
{
	int n, sign;

	while (isspace(*s))		/* skip white space */
		s++;
	sign = (*s == '-') ? -1 : 1;
	if (*s == '-' || *s == '+')	/* skip sign */
		s++;
	for (n = 0; isdigit(*s); s++)
		n = 10 * n + (*s - '0');
	return sign * n;
}
