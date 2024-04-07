/*
 * Exercise 3-6. Write a version of itoa that accepts three arguments instead
 * of two. The third argument is a minimum field width; the converted number
 * must be padded with blanks on the left if necessary to make it wide enough.
*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINE 1000

void itoa(int n, char s[], int min);
void reverse(char s[]);

/* testing itoa */
int main()
{
	int n;
	char s[MAXLINE];

	printf("-----itoa(n,s,10)----\n");
	for (n = 1; n <= 10000000; n *= 11) {
		itoa(n,s,10);
		printf("%10d:%s\n", n, s);
	}
	return 0;
}

void itoa(int n, char s[], int w)
{
	int i, sign;

	if ((sign = n) < 0) {	/* record sign */
		if (n == INT_MIN)
			n++;
		n = -n;		/* make n positive */
	}
	i = 0;
	do {	/* generate digits in reverse order */
		s[i++] = n % 10 + '0';	/* get next digit */
	} while ((n /= 10) > 0);	/* delete it */
	if (sign < 0) {
		s[i++] = '-';
		if (sign == INT_MIN)
			s[0]++;
	}
	while (i < w)		/* padd with blanks */
		s[i++] = ' ';
	s[i] = '\0';
	reverse(s);
}

/* reverse: reverse string s in place */
void reverse(char s[])
{
	int c, i, j;

	for (i = 0, j = strlen(s)-1; i < j; i++, j--)
		c = s[i], s[i] = s[j], s[j] = c;
}
