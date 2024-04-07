#include <stdio.h>

int main()
{
	return 0;
}

/* myfputs: put string s on file iop */
int myfputs(char *s, FILE *iop)
{
	int c;

	while (c = *s++)
		putc(c, iop);
	return ferror(iop) ? EOF : 0;
}
