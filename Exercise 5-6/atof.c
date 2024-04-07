/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

#include <ctype.h>

/* atof: convert string s to double */
double atof(char *s)
{
	double val, power;
	int sign, exp, expsign;

	while (isspace(*s))	/* skip white space */
		++s;
	sign = (*s == '-') ? -1 : 1;
	if (*s == '+' || *s == '-')
		++s;
	for (val = 0.0; isdigit(*s); s++)
		val = 10.0 * val + (*s - '0');
	if (*s == '.')
		++s;
	for (power = 1.0; isdigit(*s); s++) {
		val = 10.0 * val + (*s - '0');
		power *= 10.0;
	}
	if (*s == 'e' || *s == 'E') {
		++s;
		expsign = (*s == '-') ? -1 : 1;
		if (*s == '-' || *s == '+')
			++s;
		for (exp = 0; isdigit(*s); s++)
			exp = 10 * exp + (*s - '0');
		exp *=expsign;
		for ( ; exp > 0; exp--)
			power /= 10;
		for ( ; exp < 0; exp++)
			power *= 10;
	}
	return sign * val / power;
}
