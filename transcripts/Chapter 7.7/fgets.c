#include <stdio.h>

int main()
{
	return 0;
}

/* myfgets: get at most n chars from iop */
char *myfgets(char *s, int n, FILE *iop)
{
	register int c;
	register char *cs;

	cs = s;
	while (--n > 0 && (c = getc(iop)) != EOF)
		if ((*cs++ = c) == '\n')
			break;
	*cs = '\0';
	return (c == EOF && cs == s) ? NULL : s;
}
