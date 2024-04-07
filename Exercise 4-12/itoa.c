/*
 * Exercise 4-12. Adapt the ideas of printd to write a recursive version of
 * itoa; that is, convert an integer into a string by calling a recursive
 * routine.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define MAXLINE 1000

void itoa(char s[], int n);

int main()
{
	char s[MAXLINE];

	itoa(s, INT_MIN);
	printf("%d %s\n",INT_MIN, s);
	return 0;
}

int itoa_r(char s[], int n, int i);

/* itoa: convert integer n into string s */
void itoa(char s[], int n)
{
	int i = 0;

	if (n < 0) {
		s[i++] = '-';
		if (n == INT_MIN) {
			i = itoa_r(s, (n=-(n+1))/10, i);
			s[i++] = n % 10 + '0' + 1;
			s[i] = '\0';
		} else {
			itoa_r(s, -n, i);
		}
	} else {
		itoa_r(s, n, i);
	}
}

/* itoa_r: recursive itoa, for n >= 0 */
int itoa_r(char s[], int n, int i)
{
	if (n / 10)
		i = itoa_r(s, n / 10, i);
	s[i++] = n % 10 + '0';
	s[i] = '\0';
	return i;
}
