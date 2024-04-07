/*
 * Exercise 5-20. Expand dcl to handle declarations with function argument
 * types, qualifiers like const, and so on.
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 40	/* kept small to unget 2 tokens without error check */
#define MAXOUT 1000	/* max size of output; excluding datatype */
#define MAXTYPE 52	/* maximum size of any datatype (specifiers) */

enum { NO, YES };
enum { ERROR, NAME, TYPE, QUAL, STO };

char *alloc(int);
void afree(char *);

int getch(void);
void ungetch(int);

int dcl(void);
int dirdcl(void);

int getdatatype(char *);
int gettoken(void);
void ungettoken(char *);
int tokentype = '\n';		/* type of last token */
char token[MAXTOKEN];		/* last token string */
char datatype[MAXTYPE];		/* data type = char, int, etc. */
char out[MAXOUT];		/* output string */
char *outp;			/* next position to write to out */
char *outlim = out + MAXOUT;	/* out limit */
int allowanon;			/* are unnamed values allowed? (inside args) */
int alertvoid;			/* alert "void" type is incomplete */

int main()	/* convert declaration to words */
{
	while (getdatatype(datatype) != EOF) {	/* 1st token on line */
		if (tokentype == TYPE) {	/* is the datatype */
			*(outp = out) = '\0';
			if (dcl() == ERROR)
				;	/* error msg printed already */
			else if (tokentype != '\n' && tokentype != EOF)
				printf("syntax error\n");
			else
				printf("%s %s]\n", out, datatype);
		}
		while (tokentype != '\n' && tokentype != EOF)	/* clear */
			tokentype = getch();		/* line after error */
		allowanon = alertvoid = NO;
	}
	return 0;
}

#define MINLEN 11		/* length of the shortest starmsg string */
#define MAXSTAR MAXOUT/MINLEN	/* only as big as MAXOUT can take */

int error(char *);
char *stpcpy_s(char *to, char *lim, char *from);

/* dcl: parse a declarator */
int dcl(void)
{
	int i, ns;
	enum { STAR, CONST, VOL, BOTH };
	static char *starmsg[MAXSTAR];
	static char **p = starmsg, **plim = starmsg+MAXSTAR;
	static char *pmsg[] = {
		" pointer to",
		" const pointer to",
		" volatile pointer to",
		" const volatile pointer to",
	};

	gettoken();
	for (ns = 0; tokentype == '*'; ns++) {	/* count *'s */
		if (p >= plim)
			return error("input too big");
		alertvoid = NO;
		i = STAR;
		while (gettoken() == QUAL)
			if (*token == 'c')	/* const */
				i = (i==STAR) ? CONST : (i==VOL) ? BOTH : i;
			else			/* volatile */
				i = (i==STAR) ? VOL : (i==CONST) ? BOTH : i;
		*p++ = pmsg[i];		/* store msg */
	}
	if (tokentype == ERROR || dirdcl() == ERROR)
		return ERROR;
	while (ns-- > 0 && outp < outlim)
		outp = stpcpy_s(outp, outlim, *--p);
	return (outp < outlim) ? tokentype : error("input too big");
}

/* dirdcl: parse a direct declarator */
int dirdcl(void)
{
	int type, paren(void), brack(void);

	if (tokentype == '(') {	/* ( dcl ) or anon function */
		type = tokentype;
		if (allowanon) {		/* check for anon function */
			type = gettoken();
			ungettoken(token);
		}
		if (type!=TYPE && type!=QUAL && type!=STO && type!=')') {
			if (dcl() == ERROR)
				return ERROR;
			if (tokentype != ')')
				return error("missing )");
		} else if (type != ERROR) {	/* its an anon function */
			ungettoken("(");
			outp = stpcpy_s(outp, outlim, "[<unnamed>:");
		}
	} else if (tokentype == NAME) {		/* variable name */
		outp = stpcpy_s(outp, outlim, "[");
		outp = stpcpy_s(outp, outlim, token);
		outp = stpcpy_s(outp, outlim, ":");
	} else if (allowanon) {			/* anon param var */
		outp = stpcpy_s(outp, outlim, "[<unnamed>:");
		ungettoken(token);
	} else {
		return error("expected name or (dcl)");
	}
	while ((type=gettoken()) == '(' || type == '[')
		if ((type=='(' ? paren() : brack()) == ERROR)
			return ERROR;
	if (type == ERROR)
		return ERROR;
	if (alertvoid)
		return error("invalid use of void type");
	return (outp < outlim) ? type : error("input too big");
}

/* paren: parse the inside of function parenthesis */
int paren(void)
{
	int expectargs, paramdcl(void);

	alertvoid = NO;
	expectargs = (gettoken() == ')') ? NO : YES;	/* () */
	if (expectargs && strcmp(token, "void") == 0) {	/* (void) */
		if (gettoken() == ')') {
			expectargs = NO;
		} else {
			ungettoken(token);
			ungettoken("void");
		}
	} else if (expectargs) {			/* (args) */
		ungettoken(token);
	}
	outp = stpcpy_s(outp, outlim, " function taking");
	outp = stpcpy_s(outp, outlim, expectargs ?
			" argument " : " no arguments");
	if (tokentype == ERROR || expectargs && paramdcl() == ERROR)
		return ERROR;
	outp = stpcpy_s(outp, outlim, " returning");
	return (outp < outlim) ? tokentype : error("input too big");
}

char *stpcpy(char *, char *);

/* brack: parse the inside of brackets */
int brack(void)
{
	int c;

	outp = stpcpy_s(outp, outlim, " array[");
	while (outp<outlim-4 && (c=getch())!=EOF && c!='\n')
		if ((*outp++ = c) == ']') {
			outp = stpcpy(outp, " of");
			return tokentype;
		}
	if (outp<outlim-4 && c == '\n')	/* if c was initialized */
		ungetch(c);
	return error((outp<outlim-4) ? "missing ]" : "input too big");
}

/* paramdcl: parse the parameters inside a function declaration */
int paramdcl(void)
{
	char *paramdatatype;

	if ((paramdatatype = alloc(MAXTYPE)) == NULL)
		return error("input too big");
	allowanon = YES;
	do {
		if (getdatatype(paramdatatype)==ERROR || dcl()==ERROR
				|| alertvoid==YES) {
			afree(paramdatatype);
			return ERROR;
		}
		outp = stpcpy_s(outp, outlim, " ");
		outp = stpcpy_s(outp, outlim, paramdatatype);
		outp = stpcpy_s(outp, outlim, "]");
		if (tokentype == ',')
			outp = stpcpy_s(outp, outlim, " and argument ");
	} while (tokentype == ',' && outp < outlim);
	afree(paramdatatype);
	if (outp < outlim)
		return (tokentype == ')') ? tokentype : error("missing )");
	return error("input too big");
}

int gettoken(void)	/* return next token */
{
	int c;
	char *p, *plim, *find(char *, char **);
	static char *quals[] = { "volatile", "const", NULL };
	static char *stos[] = { "auto", "register", "static", "extern", NULL };
	static char *types[] = { "void", "char", "short", "int", "long",
		"float", "double", "signed", "unsigned", NULL
	};

	if (tokentype == ERROR)
		return tokentype;
	while ((c = getch()) == ' ' || c == '\t')
		;
	plim = (p = token) + MAXTOKEN;
	if (isalpha(c) || c == '_') {
		for (*p++ = c; (isalnum(c=getch()) || c=='_') && p<plim-1; )
			*p++ = c;
		*p = '\0';
		ungetch(c);
		tokentype = (isalnum(c) || c=='_') ? error("input too big") :
			find(token, types) ? TYPE :
			find(token, quals) ? QUAL :
			find(token, stos) ? STO : NAME;
	} else {
		tokentype = *p++ = c;
		*p = '\0';
	}
	return tokentype;
}

int getdatatype(char *s)	/* return next datatype */
{
	char *p;
	int type, foundtype, sto, qual, checktype(char **);

	*(p = s) = '\0';
	foundtype = sto = qual = 0;
	while ((type=gettoken())==TYPE || type==QUAL || type==STO) {
		if (type == TYPE) {
			if (foundtype++ || checktype(&p) == ERROR)
				return tokentype = error("invalid datatype");
		} else if (type == STO) {
			if (sto++)
				return tokentype = error("invalid datatype");
			p = stpcpy(p, token);
		} else if (qual!='c'+'v' && qual!=*token) {
			p = stpcpy(p, token);
			qual += *token;
		}
		p = stpcpy(p, " ");
	}
	*--p = '\0';		/* remove last space */
	if (tokentype == ERROR)
		return ERROR;
	ungettoken(token);
	return (*s == '\0' && tokentype == EOF) ? EOF :
		(tokentype = (*s == '\0') ? error("invalid datatype") : TYPE);
}

#define MAXTYPS 3		/* maximum # of type specifiers */

/* checktype: return TYPE if valid, ERROR otherwise */
int checktype(char **ps)
{
	int i, parsetype(char *, char **);
	char id[MAXTYPS+1], *p, **pt;	/* ids: initials, exept h: short */
	static char *ids[] = {"ld","uc","uli","uhi","sc","sli","shi"};

	if ((i=parsetype(id, ps)) == 1)	/* only one type specifier */
		return TYPE;
	if (id[0]==id[1] || i == 3 && (id[0]==id[2] || id[1]==id[2]))
		return ERROR;	/* repeting specifiers are not valid */
	pt = ids;
	do {	/* all chars in id must be found; in any order */
		for (p = id; *p && strchr(*pt, *p); p++)
			;
	} while (*p && *++pt);
	return *p ? ERROR : TYPE;
}

/* parsetype: copy types into *p and capture id; return TYPES read */
int parsetype(char *id, char **p)
{
	int i = 0;
	
	do {
		if (*token == 'v')				/* void */
			alertvoid = YES;
		id[i++] = (token[0]=='s' && token[1]=='h') ?	/* short */
				token[1] : token[0];
		*p = stpcpy(*p, token);
		*p = stpcpy(*p, " ");
	} while (i<MAXTYPS && gettoken()==TYPE);
	if (tokentype != TYPE)
		ungettoken(token);
	id[i] = *--*p = '\0';	/* remove last space */
	return i;
}

/* ungettoken: pushes back s into input, with padding to separate tokens */
void ungettoken(char *s)
{
	int i;

	if (tokentype != ERROR) {
		ungetch(' ');
		for (i = strlen(s)-1; i >= 0; --i)
			ungetch(s[i]);
	}
}

/* find: return pointer to s in list if found; return NULL otherwise */
char *find(char *s, char **list)
{
	do {
		if (strcmp(s, *list) == 0)
			return *list;
	} while (*++list);
	return NULL;
}

/* error: print error message s and return ERROR */
int error(char *s)
{
	printf("error: %s\n", s);
	return ERROR;
}

/* strpcpy: copy t to s; return pointer to end of s */
char *stpcpy(char *s, char *t)
{
	while (*s = *t)
		s++, t++;
	return s;
}

/* stpcpy_s: copy t to s, up to lim; return pointer to end of s */
char *stpcpy_s(char *s, char *lim, char *t)
{
	while (s < lim && (*s = *t))
		s++, t++;
	return s;
}
