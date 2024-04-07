#include <stdio.h>
#include <ctype.h>
#include "stream.h"

struct intfun getc_f = { (INTFUN) getchar, NULL };
struct cifun make_f = { (CIFUN) make_ci, (void *) &getc_f };
struct stream strm_in = { &make_f };

struct strminfo mode_nfo = { &strm_in, CODE };
struct cifun mode_f = { (CIFUN) mode_ci, (void *) &mode_nfo };
struct cifun code_f = { (CIFUN) code_ci, (void *) &mode_f };
struct stream strm_code = { &code_f };

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

/* code_ci: get charinfo from p; filtering comments, quotes and preproc */
struct charinfo code_ci(struct cifun *f)
{
        int m;
        struct charinfo ci;

        do {
                ci = f->fun(f->args);
                if (ci.mode == PREPROC)
                        while ((ci=f->fun(f->args)).c != '\n' && ci.c != EOF)
                                ;
                m = ci.mode;
        } while (m!=CODE && m!=OPCOMMENT && m!=CLCOMMENT && ci.c!=EOF);
        return ci;
}

/* getword: get next word or character from strm */
int getword(struct stream *strm, char *word, int lim)
{
	int c;
	struct charinfo ci;
	char *w = word;

	while (isspace(c = (ci=getci(strm)).c))
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
