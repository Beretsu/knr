/*
 * Exercise 2-10. Rewrite the function lower, which converts upper case letters
 * to lower case, with a conditional expression instead of if-else.
*/

#include <stdio.h>
#include <limits.h>

int lower(int c);

/* testing lower; print upper and lower character pairs */
int main()
{
	int c, low;

	for (c = 0; c <= SCHAR_MAX; ++c)
		if ((low = lower(c)) != c)
			printf("%c : %c\n", c, low);
	return 0;
}

/* lower: convert c to lower case; ASCII only */
int lower(int c)
{
	return (c >= 'A' && c <= 'Z') ? (c-'A'+'a') : c;
}
