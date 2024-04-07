/*
 * Exercise 4-11. Modify getop so that it doesn't need to use ungetch. Hint:
 * use an internal static variable.
*/

/*
 * getop bugs:
 *      a lone '.' is a NUMBER.
 *      "-." is not a NUMBER.
 * I didn't fix this since the sugestion for a static variable seems to expect
 * only one character of pushback.
*/

#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include "calc.h"
#define MAXOP 100	/* max size of operand or operator */

/* reverse Polish calculator */
int main()
{
	int type;
	double op2;
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case '+':
			push(pop() + pop());
			break;
		case '*':
			push(pop() * pop());
			break;
		case '-':
			op2 = pop();
			push(pop() - op2);
			break;
		case '/':
			op2 = pop();
			if (op2 != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '\n':
			printf("\t%.8g\n", pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}
