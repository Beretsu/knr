#include <stdio.h>
#include "stream.h"

/* getci: get a (possibly pushed back) charinfo from stream strm */
struct charinfo getci(struct stream *strm)
{
	struct charinfo ci;

	if (strm->bufstate == FULL) {
		strm->bufstate = EMPTY;
		ci = strm->buf;
	} else {
		ci = strm->pfun->fun(strm->pfun->args);
	}
	return ci;
}

/* ungetci: push back charinfo on input stream strm */
struct charinfo ungetci(struct stream *strm, struct charinfo ci)
{
	if (strm->bufstate == FULL) {
		printf("ungetch: too many characters\n");
		return ci;
	} else if (ci.c == EOF) {
		return ci;
	} else {
		strm->bufstate = FULL;
		return strm->buf = ci;
	}
}
