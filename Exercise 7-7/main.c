/*
 * Exercise 7-7. Modify the pattern finding program of Chapter 5 to take its
 * input from a set of named files or, if no files are named as arguments, from
 * the standard input. Should the file name be printed when a matching line is
 * found?
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000
#define EXEPT 1
#define NUMBER 2
#define PATH 4

/* find: print lines that match pattern from 1st arg */
int main(int argc, char *argv[])
{
	char line[MAXLINE];
	long int lineno = 0;
	char options = 0;
	int c, found = 0, matchlnprint(char *, char *, FILE *, char);
	char *prog = argv[0];
	
	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch(c) {
			case 'x':
				options |= EXEPT;
				break;
			case 'n':
				options |= NUMBER;
				break;
			case 'p':
				options |= PATH;
				break;
			default:
				fprintf(stderr, "%s: illegal option %c\n",
						prog, c);
				argc = 0;
				found = -1;
				break;
			}

	if (argc < 1)
		fprintf(stderr,
			"Usage: find -x -n -p pattern file1 file2 ...\n");
	else if (argc == 1) {
		found += matchlnprint(*argv, "stdin", stdin, options);
	} else {
		char *pattern = *argv;
		char *path;
		FILE *fp;
		while (--argc > 0) {
			path = *++argv;	
			if ((fp = fopen(path, "r")) == NULL) {
				fprintf(stderr, "%s: can't open %s\n",
						prog, path);
				exit(1);
			} else
				found += matchlnprint(pattern, path, fp,
						options);
			fclose(fp);
		}
	}
	return found;
}

int matchlnprint(char *pattern, char *path, FILE *fp, char opt)
{
	int found = 0;
	long int lineno = 0;
	char s[MAXLINE];

	while (fgets(s, MAXLINE, fp) != NULL) {
		++lineno;
		if ((strstr(s, pattern) != NULL) != (opt & EXEPT)) {
			if (opt & PATH)
				printf("%s:", path);
			if (opt & NUMBER)
				printf("%ld:", lineno);
			printf("%s", s);
			found++;
		}
	}
	return found;
}
