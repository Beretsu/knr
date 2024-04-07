/*
 * Exercise 1-9. Write a program to copy its input to its output, replacing
 * each string of one or more blanks by a single blank.
*/

#include <stdio.h>

#define YES 1
#define	NO 0

int main()
{
	int c, ignore;

	ignore = NO;
	while ((c = getchar()) != EOF) {
		if (c == ' ') {
			if (ignore == NO) {
				putchar(c);
				ignore = YES;
			}
		}
		if (c != ' ') {
			putchar(c);
			ignore = NO;
		}
	}
}
