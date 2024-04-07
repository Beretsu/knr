/*
 * Exercise 5-2. Write getfloat, the floating-point analog of getint. What type
 * does getfloat return as its function value?
*/

/*
 * getfloat bug:
 * 	consumes white space even if its input is not a number.
*/

#include <stdio.h>
#include <ctype.h>

void ungets(char []);
int getch(void);
void ungetch(int);

/* getfloat: get next float from input into *pn */
int getfloat(double *pn)
{
	int c, sign, exp, expsign, isnumber;
	double power;
	char ahead[5];			/* keep track of looked ahead chars */
	int i = 0;			/* index for ahead */

	while (isspace(c = getch()))		/* skip white space */
		;
	if (c == EOF)
		return c;
	if (!isdigit(c) && c != '-' && c != '+' && c != '.') {
		ungetch(c);			/* it is not a number */
		return 0;
	}
	sign = (c == '-') ? -1 : 1;		/* store sign */
	if (c == '-' || c == '+') {
		ahead[i++] = c;
		c = getch();
	}
	if (isnumber = isdigit(c))		/* has integer part */
		i = 0;
	for (*pn = 0.0; isdigit(c); c = getch())	/* integer part */
		*pn = 10.0 * *pn + (c - '0');
	if (c == '.') {
		if (isdigit(c=getch()))		/* has fractional part */
			i = !(isnumber = 1);
		else
			ahead[i++] = '.';
	}
	for (power = 1.0; isdigit(c); c = getch()) {	/* fractional part */
		*pn = 10.0 * *pn + (c - '0');
		power *= 10.0;
	}
	if (isnumber && (c == 'E' || c == 'e')) {	/* exponent */
		ahead[i++] = c;
		expsign = ((c=getch())=='-') ? -1 : 1;	/* store sign */
		if (c == '+' || c == '-') {
			ahead[i++] = c;
			c = getch();
		}
		for (exp = 0; isdigit(c); c = getch())	/* get digits */
			exp = 10 * exp + (c - '0');
		if ((exp *= expsign) > 0) { 		/* valid exponent */
			i = 0;
			for ( ; exp > 0; exp--)
				power /= 10;
			for ( ; exp < 0; exp++)
				power *= 10;
		}
	}
	if (c != EOF)
		ahead[i++] = c;
	ahead[i] = '\0';
	ungets(ahead);
	*pn = sign * *pn / power;
	return isnumber ? c : 0;
}
