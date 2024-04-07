/*
 * Exercise 1-20. Write a program detab that replaces tabs in the input with
 * the proper number of blanks to space to the next tab stop. Assume a fixed
 * set of tab stops, say every n columns. Should n be a variable or a symbolic
 * parameter?
*/

/*
 * Other solutions using TABSIZE-pos%TABSIZE or pos/TABSIZE*TABSIZE+TABSIZE-pos
 * can suffer from buffer overflow. This solution avoids that.
*/

#include <stdio.h>
#define TABSIZE 8

int main()
{
	int c;
	int pos;	/* position relative to TABSIZE */

	pos = 0;
	while ((c = getchar()) != EOF) {
		if (c == '\t') {
			while (pos < TABSIZE) {
				putchar(' ');
				pos++;
			}
			pos = 0;
		} else {
			putchar(c);
			if (c == '\n' || pos == TABSIZE-1)
				pos = 0;
			else
				++pos;
		}
	}
	return 0;
}
