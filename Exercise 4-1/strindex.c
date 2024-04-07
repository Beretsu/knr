/*
 * Exercise 4-1. Write the function strindex(s,t) which returns the position of
 * the rightmost occurrence of t in s, or -1 if there is none.
*/

#include <string.h>

int strindex(char s[], char t[])
{
	int i, j, k;
	int tlen = strlen(t)-1;

	for (i = strlen(s)-1; i >= tlen; i--) {
		for (j=i, k=tlen; k>=0 && s[j]==t[k]; j--, k--)
			;
		if (k!=tlen && k==-1)	/* t not empty and all chars match */
			return i - tlen;
	}
	return -1;
}
