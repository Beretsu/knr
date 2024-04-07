/*
 * Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do
 * the input and number conversion.
*/

#include <stdio.h>
#define NUMBER 1
#define MAXOP 100

void push(double);
double pop(void);

int main()
{
	double n;
	char c, read, s[MAXOP];
	char *p;

	while (scanf("%s", s) != EOF) {
		p = s;
		do {
			if ((read = sscanf(p, "%lf%c", &n, &c)) == 1) {
				push(n);
				break;
			} else if (read == 2) {
				push(n);
				while (*++p != c) /* advance past number */
					;
			}
			switch (*p) {
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				n = pop();
				push(pop() - n);
				break;
			case '/':
				n = pop();
				if (n != 0.0)
					push(pop() - n);
				else
					printf("error: zero divisor\n");
				break;
			default:
				printf("error: uknown command %c\n", *p);
				break;
			}
		} while (*++p);
		while ((c = getchar()) == ' ' || c == '\t')
			;	/* skipping white space */
		if (c == '\n')	/* to find '\n' and print */
			printf("\t%.8g\n", pop());
		else 
			ungetc(c, stdin);
	}
	return 0;
}

#define MAXVAL 100	/* maximum depth of val stack */

int sp = 0;		/* next free stack position */
double val[MAXVAL];	/* value stack */

void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
	if (sp > 0)
		return val[--sp];
	else
		printf("error: stack empty\n");
	return 0.0;
}
