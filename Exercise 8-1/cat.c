/*
 * Exercise 8-1. Rewrite the program cat from Chapter 7 using read, write,
 * open and close instead of their standard library equivalents. Perform
 * experiments to determine the relative speeds of the two versions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#undef BUFSIZ
#include "syscalls.h"

void error(char *, ...);

/* cat: concatenate files, system calls version */
int main(int argc, char *argv[])
{
	int fd;
	void filecopy(int fd1, int fd2);

	if (argc == 1)	/* no args; copy standard input */
		filecopy(0, 1);
	else
		while (--argc > 0) {
			if ((fd = open(*++argv, O_RDONLY, 0)) == -1)
				error("cat: can't open %s", *argv);
			else {
				filecopy(fd, 1);
				close(fd);
			}
		}
	if (ferror(stdout))
		error("cat: writing stdout");
	exit(0);
}

/* copy from file descriptor fd1 to fd2 */
void filecopy(int fd1, int fd2)
{
	char buf[BUFSIZ];
	int n;

	while ((n = read(fd1, buf, sizeof buf)) > 0)
		if (write(fd2, buf, n) != n)
			error("cat: writing to file");
}
