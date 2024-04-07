#include <stdio.h>
#undef BUFSIZ
#include "syscalls.h"
#include <fcntl.h>

void error(char *, ...);

int main(int argc, char *argv[])
{
	int fd, i, n, get(int fd, long pos, char *buf, int n);
	char buf[BUFSIZ];

	if ((fd = open("syscalls.h", O_RDONLY, 0)) == -1)
		error("can't open syscalls.h");
	while (read(fd, buf, 1) != 0)
		write(1, buf, 1);
	if ((n = get(fd, 0, buf, 5)) >= 0) {
		printf("get first five: ");
		for (i = 0; i < n-1; ++i)
			printf("%c, ", buf[i]);
		printf("%c\n", buf[i]);
	} else {
		error("reading from file\n");
	}
	return 0;
}

/* get: read n bytes from position pos */
int get(int fd, long pos, char *buf, int n)
{
	if (lseek(fd, pos, 0) >= 0)	/* get to pos */
		return read(fd, buf, n);
	else
		return -1;
}
