/*
 * Exercise 1-23. Write a program to remove all comments from a C program.
 * Don't forget to handle quoted strings and character constants properly.
 * C comments don't nest.
*/

#include <stdio.h>

#define PROGRAM 0		/* regular program text */
#define SLASH	1		/* '/' outside comment */
#define COMMENT	2		/* inside comment */
#define STAR	3		/* '*' inside comment */
#define QUOTE	4		/* inside single or double quotes */
#define BKSLASH	5		/* '\\' inside quotes */

int main()
{
	int c;
	int state;		/* PROGRAM, COMMENT, etc */
	char quote;		/* ' or " */

	state = PROGRAM;
	while ((c = getchar()) != EOF) {
		if (state == PROGRAM) {
			if (c == '/') {
				state = SLASH;
			} else if (c == '"' || c == '\'') {
				state = QUOTE;
				putchar(quote = c);
			} else {
				putchar(c);
			}
		} else if (state == SLASH) {
			if (c == '*') {
				state = COMMENT;
			} else if (c == '/') {
				putchar(c);
			} else if (c == '\'' || c == '"') {
				state = QUOTE;
				putchar(quote = c);
			} else {
				state = PROGRAM;
				putchar('/');
				putchar(c);
			}
		} else if (state == COMMENT) {
			if (c == '*')
				state = STAR;
		} else if (state == STAR) {
			if (c == '/')
				state = PROGRAM;
			else if (c != '*')
				state = COMMENT;
		} else if (state == QUOTE) {
			if (c == '\\')
				state = BKSLASH;
			else if (c == quote)
				state = PROGRAM;
			putchar(c);
		} else {				/* state == BKSLASH */
			putchar(c);
			state = QUOTE;
		}
	}
	if (state == SLASH)
		putchar('/');
	return 0;
}
