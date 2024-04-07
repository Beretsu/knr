/*
 * Exercise 4-6. Add commands for handling variables. (It's easy to provide
 * twenty-six variables with single-letter names.) Add a variable for the most
 * recently printed value.
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
#include <math.h>
#include <errno.h>

#define MAXOP 100	/* max size of operand or operator */
#define ALPHA 26	/* letters in the alphabet */
#define NUMBER '0'	/* signal that a number was found */
#define FUNCTION '1'	/* signal a function call */
#define VARIABLE '2'	/* signal reading a variable */
#define ASSIGNMENT '3'	/* signal storing value into a variable */

int getop(char []);
void push(double);
double pop(void);
double top(void);
void swap(void);
void clear(void);
void callfunc(char []);

double ans = 0.0;	/* most recently printed value */

/* reverse Polish calculator */
int main()
{
	int type;
	double op2;
	double vars[ALPHA] = {0};	/* variables */
	char s[MAXOP];

	while ((type = getop(s)) != EOF) {
		switch (type) {
		case NUMBER:
			push(atof(s));
			break;
		case FUNCTION:
			callfunc(s);
			break;
		case VARIABLE:
			push(vars[s[0]-'a']);
			break;
		case ASSIGNMENT:
			vars[s[0]-'A'] = top();
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
			if ((op2 = pop()) != 0.0)
				push(pop() / op2);
			else
				printf("error: zero divisor\n");
			break;
		case '%':
			if ((op2 = pop()) != 0.0)
                                push((int)pop() % (int)op2);
                        else
                                printf("error: zero divisor\n");
                        break;
		case '\n':
			printf("\t%.8g\n", ans = pop());
			break;
		default:
			printf("error: unknown command %s\n", s);
			break;
		}
	}
	return 0;
}

void callfunc(char s[])
{
	double op2;

	if (strcmp(s, "ans") == 0) {
		push(ans);
	} else if (strcmp(s, "top") == 0) {
                printf("%.8g\n", top());
        } else if (strcmp(s, "dup") == 0) {
                push(top());
        } else if (strcmp(s, "swap") == 0) {
                swap();
        } else if (strcmp(s, "clear") == 0) {
                clear();
	} else if (strcmp(s, "sin") == 0) {
		push(sin(pop()));
	} else if (strcmp(s, "cos") == 0) {
		push(cos(pop()));
	} else if (strcmp(s, "tan") == 0) {
		push(tan(pop()));
	} else if (strcmp(s, "asin") == 0) {
		push(asin(pop()));
	} else if (strcmp(s, "acos") == 0) {
		push(acos(pop()));
	} else if (strcmp(s, "atan") == 0) {
		push(atan(pop()));
	} else if (strcmp(s, "exp") == 0) {
		push(exp(pop()));
	} else if (strcmp(s, "log") == 0) {
		push(log(pop()));
	} else if (strcmp(s, "log10") == 0) {
		push(log10(pop()));
	} else if (strcmp(s, "pow") == 0) {
		op2 = pop();
		push(pow(pop(), op2));
	} else if (strcmp(s, "sqrt") == 0) {
		push(sqrt(pop()));
	} else {
		printf("error: uknown command %s\n", s);
	}
	if (errno == EDOM || errno == ERANGE) {
		printf("%s: math error\n", s);
		errno = 0;
	}
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
		return i==1 ? (islower(s[0]) ? VARIABLE:ASSIGNMENT) : FUNCTION;
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
