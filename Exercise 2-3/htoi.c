/*
 * Execise 2-3. Write a function htoi(s), which converts a string of
 * hexadecimal digits (including an optional 0x or 0X) into its equivalent
 * integer value. The allowable digits are 0 through 9, a through f, and A
 * through F.
*/

#include <stdio.h>
#include <ctype.h>

int htoi(char s[]);

/* test htoi */
int main()
{
	char s1[] = "0x064a1d4f2";
	char s2[] = "0X40F9e";
	char s3[] = "AFD";
	char s4[] = "cfb";

	printf("%s = %d\n", s1, htoi(s1));
	printf("%s = %d\n", s2, htoi(s2));
	printf("%s = %d\n", s3, htoi(s3));
	printf("%s = %d\n", s4, htoi(s4));
	return 0;
}

/* htoi: hexadecimal to decimal */
int htoi(char s[])
{
	int c, i, n;

	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		i = 2;
	else 
		i = 0;
	n = 0;
	for ( ; isdigit(s[i]) || (c=tolower(s[i]))>='a' && c<='f'; ++i)
		if (isdigit(s[i]))
			n = 16 * n + (s[i]-'0');
		else
			n = 16 * n + (c-'a'+10);
	return n;
}

