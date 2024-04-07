/*
 * Exercise 1-21. Write a program entab that replaces strings of blanks by the
 * minimum number of tabs and blanks to achive the same spacing. Use the same
 * tab stops as for detab. When either a tab or a single blank would suffice
 * to reach a tab stop, which should be given preference?
*/

/* 
 * Single spaces are not replaced if they alone suffice to reach a tab stop.
 * Its a better guess for preseving the intentions of the original text.
 *
 * Other solutions using TABSIZE-pos%TABSIZE or pos/TABSIZE*TABSIZE+TABSIZE-pos
 * can suffer from buffer overflow. This solution avoids that.
*/

#include <stdio.h>
#define TABSIZE 8

int main()
{
	int c;
	int pos;	/* position relative to TABSIZE */
	int spaces;	/* # of spaces adding to a tab */

	pos = spaces = 0;
	while ((c = getchar()) != EOF) {
		if (c=='\t' || c==' ' && pos==TABSIZE-1) {
			if (c==' ' && spaces == 0)
				c = ' ';
			else
				c = '\t';
			putchar(c);
			pos = spaces = 0;
		} else if (c == ' ') {
			++pos;
			++spaces;
		} else {
			while (spaces > 0) {
				putchar(' ');
				--spaces;
			}
			putchar(c);
			if (c == '\n' || pos == TABSIZE-1)
				pos = 0;
			else
				++pos;
		}
	}
	while (spaces > 0) {		/* if EOF with no '\n' before it */
		putchar(' ');
		--spaces;
	}
	return 0;
}
