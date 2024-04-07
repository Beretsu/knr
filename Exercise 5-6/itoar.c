/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

#include <limits.h>

char *itoa_r(char *s, int n);

/* itoa: convert integer n into string s */
void itoa(char *s, int n)
{
	if (n < 0) {
		*s++ = '-';
		if (n == INT_MIN) {
			s = itoa_r(s, (n=-(n+1))/10);
			*s++ = n % 10 + '0' + 1;
			*s = '\0';
		} else {
			itoa_r(s, -n);
		}
	} else {
		itoa_r(s, n);
	}
}

/* itoa_r: recursive itoa, for n >= 0 */
char *itoa_r(char *s, int n)
{
	if (n / 10)
		s = itoa_r(s, n / 10);
	*s++ = n % 10 + '0';
	*s = '\0';
	return s;
}
