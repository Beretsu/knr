/*
 * Exercise 5-10. Write the program expr, which evaluates a reverse Polish
 * expression from the command line, where each operator or operand is a
 * separate argument. For example,
 * 	expr 2 3 4 + *
 * evaluates 2 * (3+4).
*/

/*
 * from clc-wiki.net:
 * "The reason that * is causing issues is that it is a shell wildcard. You are
 * calling the program from your shell and the argument is passing to the shell
 * first, which expands it into a list of all files in the current folder. If
 * you use the escape sequence \* instead of * as an argument while calling your
 * c program, you should get the expected result.). However, if you escape the
 * asterisk and you are still having problems, it could be that you are using
 * MinGW's version of gcc. See anonymous' solution to that problem."
*/

#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#define NUMBER '0'

int getop(char *);
void push(double);
double pop(void);

/* reverse Polish calculator */
int main(int argc, char *argv[])
{
	double op2;

	if (argc < 2) {
		printf("Usage: %s op op ...\n", *argv);
		return 0;
	}
	while (--argc) {
		switch (getop(*++argv)) {
		case NUMBER:
			push(atof(*argv));
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
		default:
			printf("error: unknown command %c\n", **argv);
			break;
		}
	}
	printf("\t%.8g\n", pop());
	return 0;
}

#include <ctype.h>

/* getop: get operator or numeric operand from s */
int getop(char *s)
{
	int c;

	while ((c = *s++) == ' ' || c == '\t')
		;
	if (isdigit(c) || c == '.' && isdigit(*s))
		return NUMBER;
	else if (c == '-' || c == '+')
		if (isdigit(*s++) || *s == '.' && isdigit(*++s))
			return NUMBER;
	return c;
}
