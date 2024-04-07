/*
 * Exercise 5-19. Modify undcl so that it does not add redundant parentheses to
 * declarations.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };
enum { OFF, ON };

int gettoken(void);
int tokentype;			/* type of last token */
char token[MAXTOKEN];		/* last token string */
char name[MAXTOKEN];		/* identifier name */
char datatype[MAXTOKEN];	/* datatype = char, int, etc. */
char out[1000];			/* output string */

/* undcl: convert word description to declaration */
int main()
{
	int type, star;		/* star: signal '*' to question its priority */
	char temp[MAXTOKEN];

	star = OFF;
	while (gettoken() != EOF) {
		strcpy(out, token);
		while ((type = gettoken()) != '\n') {
			if (star = ON) {
				if (type == PARENS || type == BRACKETS)
					sprintf(temp, "(*%s)", out);
				else
					sprintf(temp, "*%s", out);
				strcpy(out, temp);
				star = OFF;
			}
			if (type == PARENS || type == BRACKETS)
				strcat(out, token);
			else if (type == '*') {
				star = OFF;
			} else if (type == NAME) {	/* datatype */
				sprintf(temp, "%s %s", token, out);
				strcpy(out, temp);
			} else
				printf("invalid input at %s\n", token);
		}
		printf("%s\n", out);
	}
	return 0;
}

int gettoken(void)	/* return next token */
{
	int c, getch(void);
	void ungetch(int);
	char *p = token;

	while ((c = getch()) == ' ' || c == '\t')
		;
	if (c == '(') {
		if ((c = getch()) == ')') {
			strcpy(token, "()");
			return tokentype = PARENS;
		} else {
			ungetch(c);
			return tokentype = '(';
		}
	} else if (c == '[') {
		for (*p++ = c; (*p++ = getch()) != ']'; )
			;
		*p = '\0';
		return tokentype = BRACKETS;
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c = getch()); )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		return tokentype = NAME;
	} else
		return tokentype = c;
}
