/*
 * Exercise 1-24. Write a program to check a C program for rudamentary syntax
 * errors like unmatched parentheses, brackets and braces. Don't forget about
 * quotes, both single and double, escape sequences and comments.
 * (This program is hard if you do it in full generality.)
*/

#include <stdio.h>
#define TABSIZE	8	/* size of a tab */
#define PROGRAM	0	/* regular program text */
#define SLASH	1	/* '/' outside comment */
#define COMMENT	2	/* inside comment */
#define CSTAR	3	/* '*' inside comment */
#define QUOTE	4	/* inside single or double quotes */
#define BKSLASH 5	/* '\\' inside quotes */
#define PSTAR	6	/* '*' outside comment */

void push(int c);
int pop(void);
int isempty(void);
int getmatch(int);

/* reports only the first unmatched tag */
int main()
{
	int c;
	int state;	/* PROGRAM, COMMENT, etc */
	int quote;	/* ' or " */
	int line;	/* current line number */
	int col;	/* current column number */

	col = 0;
	line = 1;
	state = PROGRAM;
	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			++line;
			col = 0;
		} else if (c == '\t') {
			col = col/TABSIZE*TABSIZE+TABSIZE; /* next tabstop */ 
		} else {
			++col;
		}

		if (state == PROGRAM) {
			if (c == '(' || c == '[' || c == '{') {
				push(c);
			} else if (c == ')' || c == ']' || c == '}') {
				if (isempty() || pop() != getmatch(c)) {
					printf("%d:%d: error: unmatched %c\n",
							line, col, c);
					return -1;
				}
			} else if (c == '"' || c == '\'') {
				state = QUOTE;
				push(quote = c);
			} else if (c == '/') {
				state = SLASH;
			} else if (c == '*') {
				state = PSTAR;
			}
		} else if (state == SLASH) {
			if (c == '*') {
				state = COMMENT;
				push('/');
			} else if (c == '"' || c == '\'') {
				state = QUOTE;
				push(quote = c);
			} else if (c != '/') {
				state = PROGRAM;
			}
		} else if (state == COMMENT) {
			if (c == '*') {
				state = CSTAR;
			}
		} else if (state == CSTAR) {
			if (c == '/') {
				state = PROGRAM;
				pop();
			} else if (c != '*') {
				state = COMMENT;
			}
		} else if (state == QUOTE) {
			if (c == '\\') {
				state = BKSLASH;
			} else if (c == quote) {
				state = PROGRAM;
				pop();
			}
		} else if (state == BKSLASH) {
			state = QUOTE;
		} else {				/* state == PSTAR */
			if (c == '/') {
				printf("%d:%d: error: unmatched */\n",
						line, col);
				return -1;
			} else if (c != '*') {
				state = PROGRAM;
			}
		}
	}

	if (!isempty()) {			/* missing closing tag */
		if ((c = pop()) == '/')
			printf("%d:%d: error: unmatched /*\n", line, col);
		else
			printf("%d:%d: error: unmatched %c\n", line, col, c);
		return -1;
	}
	return 0;
}

/* getmatch: return openning counterpart to c */
int getmatch(int c)
{
	if (c == ')')
		return '(';
	else if (c == ']')
		return '[';
	else
		return '{';
}

#define BUFSIZE 1000
int buff[BUFSIZE];	/* buffer for stack */
int bufp = 0;		/* next	free position in stack */

/* push: put c into stack */
void push(int c)
{
	buff[bufp] = c;
	++bufp;
}

/* pop: remove and return top of stack */
int pop(void)
{
	--bufp;
	return buff[bufp];
}

/* isempty: return 1 if stack is empty, otherwise return 0 */
int isempty(void) {
	if (bufp == 0)
		return 1;
	return 0;
}
