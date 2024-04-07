#include <stdio.h>
#include <ctype.h>
#include "stream.h"

struct intfun getc_f = { (INTFUN) getchar, NULL };
struct cifun make_f = { (CIFUN) make_ci, (void *) &getc_f };
struct stream strm_in = { &make_f };

struct strminfo mode_nfo = { &strm_in, CODE };
struct cifun mode_f = { (CIFUN) mode_ci, (void *) &mode_nfo };
struct stream strm_mode = { &mode_f };

struct cifun ncom_f = { (CIFUN) ncom_ci, (void *) &strm_mode };
struct stream strm_ncom = { &ncom_f };

/* buf_c: get next char from array in p */
int buf_c(struct bufinfo *p)
{
	return (p->buf[p->pos] == '\0') ? EOF : p->buf[(p->pos)++];
}

/* make_ci: get charinfo; add char (from f) */
struct charinfo make_ci(struct intfun *f)
{
	struct charinfo ci;

	ci.c = f->fun(f->args);
	return ci;
}

/* mode_ci: get charinfo from p; add mode */
struct charinfo mode_ci(struct strminfo *p)
{
	int c;
	static int quote;
	struct charinfo ci1, ci2;

	ci1 = getci(p->strm);
	c = ci1.c;
	switch (ci1.mode = p->mode) {
	case CODE:
		switch (c) {
		case '#':
			ci1.mode = PREPROC;
			return ci1;
		case '/':
			if ((ci2=getci(p->strm)).c == '*')
				ci1.mode = p->mode = OPCOMMENT;
			ungetci(p->strm, ci2);
			return ci1;
		case '"': case '\'':
			p->mode = QUOTE;
			quote = c;
			return ci1;
		default:
			return ci1;
		}
	case OPCOMMENT:
		p->mode = COMMENT;
		return ci1;
	case COMMENT:
		if (c == '*') {
			if ((ci2=getci(p->strm)).c == '/')
				ci1.mode = p->mode = CLCOMMENT;
			ungetci(p->strm, ci2);
		}
		return ci1;
	case CLCOMMENT:
		p->mode = CODE;
		return ci1;
	case QUOTE:
		if (c == '\\')
			p->mode = ESC;
		else if (c == quote || c == '\n')
			ci1.mode = p->mode = CODE;
		return ci1;
	case ESC:
		ci1.mode = p->mode = QUOTE;
		return ci1;
	default:
		return ci1;
	}
}

/* ncom_ci: get charinfo from p; filtering comments */
struct charinfo ncom_ci(struct stream *p)
{
	struct charinfo ci;

	if (incomment(ci=getci(p))) {	/* replace them for a single ' ' */
		for ( ; incomment(ci) && ci.c!=EOF; ci=getci(p))
			;
		if (ci.c != EOF) {
			ungetci(p, ci);
			ci.c = ' ';
			ci.mode = SPACER;
		}
	}
	return ci;
}

/* getword: get next word or character from strm */
int getword(struct stream *strm, int linebound, char *word, int lim)
{
	int c;
	struct charinfo ci;
	char *w = word;
	int (*istrans)(int) = linebound ? isblank : isspace;

	while (istrans(c = (ci=getci(strm)).c))
		;
	if (c != EOF)
		*w++ = c;
	if (!asalpha(c)) {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; w++)
		if (!asalnum(*w = (ci=getci(strm)).c)) {
			ungetci(strm, ci);
			break;
		}
	*w = '\0';
	return word[0];
}

/* getline: get line into s, return length */
int getline(struct stream *strm, char *s, int lim)
{
	struct charinfo ci;
	char *p = s;

	while (--lim > 0 && (ci=getci(strm)).c!=EOF && ci.c!='\n')
		*s++ = ci.c;
	if (ci.c == '\n')
		*s++ = ci.c;
	*s = '\0';
	return s - p;
}

int incomment(struct charinfo ci)
{
	return (ci.mode==COMMENT || ci.mode==OPCOMMENT || ci.mode==CLCOMMENT);
}

/* asalpha: valid intials for a variable name */
int asalpha(int c)
{
        return isalpha(c) || c == '_';
}

/* asalnum: valid chars in varariable name */
int asalnum(int c)
{
        return isalnum(c) || c == '_';
}

int isblank(int c)
{
	return c == ' ' || c == '\t';
}
