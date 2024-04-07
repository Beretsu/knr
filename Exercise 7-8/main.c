/*
 * Exercice 7-8. Write a program to print a set of files, starting each new one
 * on a new page, with a title and a running page count for each file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGEBREAK 20
#define MAXLINE 1000

enum { NO_ERROR, OPEN_ERROR, WRITE_ERROR };

int main(int argc, char *argv[])
{
	FILE *fp;
	char *prog = *argv;
	void paginate(FILE *, char *);

	if (argc == 1)
		paginate(stdin, "stdin");
	else {
		while (--argc > 0) {
			if ((fp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "%s: can't open %s\n",
						prog, *argv);
				exit(OPEN_ERROR);
			}
			paginate(fp, *argv);
			fclose(fp);
		}
	}
	exit(NO_ERROR);
}

void paginate(FILE *fp, char *prog)
{
	char line[MAXLINE];
	int lineno = 0, pageno = 1;

	while (fgets(line, MAXLINE, fp) != NULL) {
		if (lineno % PAGEBREAK == 0) {
			printf("File: %-60.60s page %d\n\n", prog, pageno++);
			lineno += 2;
		}
		printf("%s", line);
		if (ferror(stdout)) {
			fprintf(stderr, "%s: can't write\n", prog);
			exit(WRITE_ERROR);
		}
		++lineno;
	}

	if (line[strlen(line)-1] != '\n') /* if file didn't end with '\n' */
		putchar('\n');		/* put it */

	while (lineno++ % PAGEBREAK != 0)	/* space out last page */
		putchar('\n');
}


