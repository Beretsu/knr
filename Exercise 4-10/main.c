/*
 * Exercise 4-10. An alternate organization uses getline to read an entire
 * input line; this makes getch and ungetch unnecessary. Revise the calculator
 * to use this approach.
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

#define MAXLINE 100	/* maximum input line */
#define ALPHA 26	/* letters in the alphabet */
#define NUMBER '0'	/* signal that a number was found */
#define FUNCTION '1'	/* signal a function call */
#define VARIABLE '2'	/* signal reading a variable */
#define ASSIGNMENT '3'	/* signal storing value into a variable */

int getline(char [], int);
int getop(char []);
void push(double);
double pop(void);
double top(void);
void swap(void);
void clear(void);
void callfunc(char []);

double ans = 0.0;	/* most recently printed value */
char line[MAXLINE];
int linep;

/* reverse Polish calculator */
int main()
{
	int type;
	double op2;
	double vars[ALPHA] = {0};	/* variables */
	char s[MAXLINE];

	while (getline(line, MAXLINE) > 0) {
		linep = 0;
		while ((type = getop(s)) != '\0') {
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

/* getop: get next operator or numeric operand */
int getop(char s[])
{
	int c, i;

	i = 0;
	while ((s[0] = c = line[linep++]) == ' ' || c == '\t')
		;
	if (isalpha(c)) {
		while (isalnum(s[++i] = c = line[linep]))
			linep++;
		s[i] = '\0';
		return i==1 ? (islower(s[0]) ? VARIABLE:ASSIGNMENT) : FUNCTION;
	}
	s[1] = '\0';
	if (!isdigit(c) && c != '.' && c != '-')
		return c;				/* not a number */
	if (c == '-')
		if (!isdigit(c=line[linep]) && c!='.')	/* minus operator */
			return '-';
		else					/* negative number */
			s[++i] = line[linep++];
	if (isdigit(c))		/* collect integer part */
		while (isdigit(s[++i] = c = line[linep++]))
			;
	if (c == '.')		/* collect fraction part */
		while (isdigit(s[++i] = c = line[linep++]))
			;
	linep--;
	s[i] = '\0';
	return NUMBER;
}

/* getline: get line into s, return length */
int getline(char s[], int lim)
{
	int i, c;

	i = 0;
	while (--lim > 0 && (c=getchar()) != EOF && c != '\n')
		s[i++] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}
