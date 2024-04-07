/*
 * Exercise 3-5. Write the function itob(n,s,b) that converts the integer n
 * into a base b character representation in the string s. In particular,
 * itob(n,s,16) formats s as a hexadecimal integer in s.
*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINE 1000

void itob(int n, char s[], int b);
void reverse(char s[]);

/* testing itob */
int main()
{
	int i, n;
	char bin[MAXLINE], oct[MAXLINE], hex[MAXLINE];

	printf("%7s  %16s  %6s  %4s\n", "decimal", "binary", "octal", "hex");
	for (n = 0x1111; n <= 0xffff; n += 0x1111) {
		itob(n, bin, 2);
		itob(n, oct, 8);
		itob(n, hex, 16);
		printf("%7d: %16s: %6s: %4s\n", n, bin, oct, hex);
	}
	return 0;
}

/* itob: convert n to base b characters in s */
void itob(int n, char s[], int b)
{
	int i, sign;

	if ((sign = n) < 0) {	/* record sign */
		if (n == INT_MIN)
			++n;
		n = -n;		/* make n positive */
	}
	i = 0;
	do {	/* generate digits in reverse order */
		s[i++] = (n%b<10) ? n%b+'0' : n%b-10+'a';	/* next digit */
	} while ((n /= b) > 0);	/* delete it */
	if (sign < 0) {
		s[i++] = '-';
		if (sign == INT_MIN)
			s[0]++;
	}
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
