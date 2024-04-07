#include "syscalls.h"
#include <stdio.h>
#undef getchar

int main()
{
	int c, getchar(void);
	while ((c = getchar()) != EOF)
		write(1, (char *)(&c), 1);
	return 0;
}

/* getchar: unbuffered single character input */
int getchar(void)
{
	char c;

	return (read(0, &c, 1) == 1) ? (unsigned char) c : EOF;
}
