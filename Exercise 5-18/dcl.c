/*
 * Exercise 5-18. Make dcl recover from input errors.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define MAXOUT 1000

enum { ERROR, NAME, PARENS, BRACKETS };

int getch(void);
void ungetch(int);

int dcl(void);
int dirdcl(void);

int gettoken(void);
int tokentype;			/* type of last token */
char token[MAXTOKEN];		/* last token string */
char name[MAXTOKEN];		/* identifier name */
char datatype[MAXTOKEN];	/* data type = char, int, etc. */
char out[MAXOUT];		/* output string */
char *outp;			/* next position to write to out */
char *outlim = out + MAXOUT;	/* out limit */

int main()	/* convert declaration to words */
{
	while (gettoken() != EOF) {		/* 1st token on line */
		if (tokentype == NAME) {	/* is the datatype */
			strcpy(datatype, token);
			*(outp = out) = '\0';
			if (dcl() == ERROR)
				;	/* error msg printed already */
			else if (tokentype != '\n' && tokentype != EOF)
				printf("syntax error\n");
			else 
				printf("%s: %s %s\n", name, out, datatype);
		} else if (tokentype != ERROR)
			printf("error: expected datatype\n");
		while (tokentype != '\n' && tokentype != EOF)	/* clear */
			tokentype = getch();		/* line after error */
	}
	return 0;
}

int error(char *);
char *stpcpy_s(char *to, char *lim, char *from);

/* dcl: parse a declarator */
int dcl(void)
{
	int ns, type;
	
	for (ns = 0; (type = gettoken()) == '*'; )	/* count *'s */
		ns++;
	if (type == ERROR || dirdcl() == ERROR)
		return ERROR;
	while (ns-- > 0 && outp < outlim)
		outp = stpcpy_s(outp, outlim, " pointer to");
	return (outp < outlim) ? type : error("input too big");
}

/* dirdcl: parse a direct declarator */
int dirdcl(void)
{
	int type;

	if (tokentype == '(') {	/* ( dcl ) */
		if (dcl() == ERROR)
			return ERROR;
		if (tokentype != ')')
			return error("missing )");
	} else if (tokentype == NAME)	/* variable name */
		strcpy(name, token);
	else
		return error("expected name or (dcl)");
	while ((type=gettoken()) == PARENS || type == BRACKETS)
		if (type == PARENS)
			outp = stpcpy_s(outp, outlim, " function returning");
		else {
			outp = stpcpy_s(outp, outlim, " array");
			outp = stpcpy_s(outp, outlim, token);
			outp = stpcpy_s(outp, outlim, " of");
		}
	return (outp < outlim) ? type : error("input too big");
}

int gettoken(void)	/* return next token */
{
	int c;
	char *p = token;
	char *plim = p + MAXTOKEN;

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
		for (*p++=c; p<plim-1 && (c=getch())!=EOF && c!='\n'; )
			if ((*p++ = c) == ']') {
				*p = '\0';
				return tokentype = BRACKETS;
			}
		if (c == '\n')
			ungetch(c);
		tokentype = error((p<plim-1) ? "missing ]" : "input to big");
	} else if (isalpha(c)) {
		for (*p++ = c; isalnum(c=getch()) && p<plim-1; )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		tokentype = isalnum(c) ? error("input to big") : NAME;
	} else
		tokentype = c;
	return tokentype;
}

/* error: print error message s and return ERROR */
int error(char *s)
{
	printf("error: %s\n", s);
	return ERROR;
}

/* stpcpy_s: copy t to s, up to lim; return pointer to end of s */
char *stpcpy_s(char *s, char *lim, char *t)
{
	while (s < lim && (*s = *t))
		s++, t++;
	return s;
}
