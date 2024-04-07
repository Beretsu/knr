/*
 * Exercise 4-4. Add the commands to print the top elements of the stack without
 * popping, to duplicate it, and to swap the top two elements. Add a command to
 * clear the stack.
*/

/*
 * getop bugs:
 *      a lone '.' is a NUMBER.
 *      "-." is not a NUMBER.
 * I didn't fix this to maintain consistency with Exercise 4-11 that seems to
 * want only one character of pushback.
*/

#include <stdio.h>
#include <stdlib.h>	/* for atof() */
#include <string.h>

#define MAXOP 100	/* max size of operand or operator */
#define NUMBER '0'	/* signal that a number was found */
#define FUNCTION '1'	/* signal a function call */

int getop(char []);
void push(double);
double pop(void);
void callfunc(char []);

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
		case FUNCTION:
			callfunc(s);
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
		case '%':
			op2 = pop();
                        if (op2 != 0.0)
                                push((int)pop() % (int)op2);
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

double top(void);
void swap(void);
void clear(void);

void callfunc(char s[])
{
	if (strcmp(s, "top") == 0)
		printf("%.8g\n", top());
	else if (strcmp(s, "dup") == 0)
		push(top());
	else if (strcmp(s, "swap") == 0)
		swap();
	else if (strcmp(s, "clear") == 0)
		clear();
	else
		printf("error: uknown command\n");
}

#define MAXVAL 100	/* maximum depth of val stack */

int sp = 0;		/* next free stack position */
double val[MAXVAL];	/* value stack */

/* push: push f into value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

/* pop: pop and return top value of stack */
double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else {
		printf("error: stack empty\n");
		return 0.0;
	}
}

/* top: return top element of the stack without popping */
double top(void)
{
        if (sp > 0) {
                return val[sp-1];
        } else {
                printf("error: stack empty\n");
                return 0.0;
        }
}

/* swap: swap top two elements of the stack */
void swap(void)
{
        if (sp > 1) {
		double temp = val[sp-1];
		val[sp-1] = val[sp-2];
		val[sp-2] = temp;
        } else {
                printf("error: not enough elements in stack to swap\n");
        }
}

/* clear the stack */
void clear(void)
{
        sp = 0;
}

#include <ctype.h>

int getch(void);
void ungetch(int);

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int c, i;

	i = 0;
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	if (isalpha(c)) {
		while (isalnum(s[++i] = c = getch()))
			;
		if (c != EOF)
			ungetch(c);
		s[i] = '\0';
		return FUNCTION;
	}
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;	/* not a number */
	if (c == '-')
		if (!isdigit(c = getch())) {	/* minus operator */
			if (c != EOF)
				ungetch(c);
			return '-';
		} else {			/* negative number */
			s[++i] = c;
		}
	if (isdigit(c))		/* collect integer part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.')		/* collect fraction part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];	/* buffer for ungetch */
int bufp = 0;		/* next free position in buf */

int getch(void)		/* get a (possibly pushed-back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)	/* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: to many characters\n");
	else
		buf[bufp++] = c;
}
