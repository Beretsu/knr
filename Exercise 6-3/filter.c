#include <stdio.h>
#include <ctype.h>
#include "stream.h"

struct intfun lowr_f = { (INTFUN) getlower, getchar };
struct cifun make_f = { (CIFUN) make_ci, (void *) &lowr_f };
struct cifuninfo line_nfo = { &make_f, 1 };
struct cifun line_f = { (CIFUN) line_ci, &line_nfo };
struct stream strm_lw = { &line_f };

int getlower(int (*fun)(void))
{
	return tolower(fun());
}

/* make_ci: get charinfo; add char (from f) */
struct charinfo make_ci(struct intfun *f)
{
	struct charinfo ci;

	ci.c = f->fun(f->args);
	return ci;
}

/* line_ci: get charinfo from p; add line number */
struct charinfo line_ci(struct cifuninfo *fi)
{
	struct cifun *pf = fi->pf;
	struct charinfo ci = pf->fun(pf->args);

	ci.lineno = (ci.c == '\n') ? ++fi->n : fi->n;
	return ci;
}

/* getword: get next word or character from strm; return line number */
int getword(struct stream *strm, struct wordinfo *pwi, int lim)
{
	int c, lineno;
	struct charinfo ci;
	char *w = pwi->word;

	while (isspace(c = (ci=getci(strm)).c))
		;
	pwi->lineno = ci.lineno;
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
	return pwi->word[0];
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
