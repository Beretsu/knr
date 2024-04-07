/*
 * Exercise 3-4. In a two's complement number representation, our version of
 * itoa does not handle the largest negative number, that is, the value of n
 * equal to -(2^(wordsize-1)). Explain why not. Modify it to print that value
 * correctly, regardless of the machine on which it runs.
*/

/*
 * Signed integers go from n = -(2^(wordsize-1)) to 2^(wordsize-1)-1.
 * abs(n) > 2^(wordsize-1)-1. It doesn't fit.
 *
 * Solutions using abs(n%10) are not portable; "The direction of
 * truncation for / and the sign of the result for % are machine-dependent for
 * negative operands." (KnR Chapter 2.5)
 * Codybartfast's has an interesting solution, but from my understanding in
 * machines where "the result of the / operator is the largest integer less than
 * the algebraic quotient" the statement n =/ 10, would create an infinite loop
 * for n = -1 as -1/10 == -1 . I don't have a machine to test this on though.
 *
 * This solution relies on the fact that the last digit of INT_MIN
 * (or of any data type) is guaranteed to not be 0 as -INT_MIN is a power of 2,
 * and the last digit of powers of 2 follow the sequence
 * {1, 2 ,4 ,8 ,6 ,2 ,4 ,8 , 6, ..., }
*/

#include <stdio.h>
#include <string.h>
#include <limits.h>
#define MAXLINE 12

void itoa(int n, char s[]);
void reverse(char s[]);

/* testing itoa version 2 */
int main()
{
        char s[MAXLINE];

        itoa(INT_MIN, s);
        printf("%s\n", s);
        return 0;
}

/* itoa: convert n to characters in s */
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0) {	/* record sign */
		if (n == INT_MIN)
			++n;
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

