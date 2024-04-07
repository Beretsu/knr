/*
 * Exercise 4-13. Write a recursive version of the function reverse(s), which
 * reverses the string s in place.
*/

#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

void reverse(char s[]);

int main()
{
	char s[MAXLINE] = "this is a test";

	printf("%s\n", s);
	reverse(s);
	printf("%s\n", s);
	return 0;
}

/* reverse_r: reverse string s in place */
void reverse_r(char s[], int left, int right)
{
	char temp;

	if (left < right) {
		temp = s[left];
		s[left] = s[right];
		s[right] = temp;
		reverse_r(s, left+1, right-1);
	}
}

/* reverse: reverse string s in place */
void reverse(char s[])
{
	reverse_r(s, 0, strlen(s)-1);
}
