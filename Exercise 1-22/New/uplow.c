/*
 * Exercise 7-1. Write a program that converts upper case to lower or lower
 * case to upper, depending on the name it is invoked with, as found in argv[O]
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	int c, dummy(int), (*fun)(int);
	char *prog, *getname(char *);

	if (strcmp(prog=getname(argv[0]), "lower") == 0)
		fun = tolower;
	else if (strcmp(prog, "upper") == 0)
		fun = toupper;
	else
		fun = dummy;

	while ((c = getchar()) != EOF)
		putchar(fun(c));
	return 0;
}

/* getname: remove path from program name. eg ./path/upper -> upper */
char *getname(char *path)
{
	char *file = path;

	while (*path)
		if (*path++ == '/')
			file = path;
	return file;
}

int dummy(int c)
{
	return c;
}
