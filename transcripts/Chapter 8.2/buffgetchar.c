#include <stdio.h>
#undef BUFSIZ
#undef getchar
#include "syscalls.h"

int main()
{
	int c, getchar(void);

	while ((c = getchar()) != EOF)
		write(1, (char *)(&c), 1);
	return 0;
}

/* getchar: simple buffered version */
int getchar(void)
{
	static char buf[BUFSIZ];
	static char *bufp = buf;
	static int n = 0;

	if (n == 0) {	/* buffer is empty */
		n = read(0, buf, sizeof buf);
		bufp = buf;
	}
	return (--n >= 0) ? (unsigned char) *bufp++ : EOF;
}
