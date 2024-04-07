/*
 * Exercise 7-6. Write a program to compare two files, printing the first line
 * where they differ.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1000

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "usage: %s file1 file2\n", argv[0]);
		exit(1);
	}

	FILE *fp1, *fp2;
	if ((fp1 = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "%s: can't open %s\n", argv[0], argv[1]);
		exit(1);
	}
	if ((fp2 = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "%s: can't open %s\n", argv[0], argv[2]);
		exit(1);
	}

	int ln = 0;
	char line1[MAXLINE], line2[MAXLINE];
	register char *r1, *r2;

	do {
		++ln;
		r1 = fgets(line1, MAXLINE, fp1);
		r2 = fgets(line2, MAXLINE, fp2);
	} while (r1 && r2 && !strcmp(r1, r2));

	if (r1 != r2) {	/* differ */
		printf("%s line %d: %s", argv[1], ln, r1 ? r1 :"<EOF>\n");
		printf("%s line %d: %s", argv[2], ln, r2 ? r1 :"<EOF>\n");
	}

	if (ferror(stdout)) {
		fprintf(stderr, "%s: error writing stdout\n", argv[0]);
		exit(2);
	}
	exit(0);
}
