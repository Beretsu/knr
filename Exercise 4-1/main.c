#include <stdio.h>
#define MAXLINE 1000	/* maximum input line length */

int getline(char line[], int max);
int strindex(char source[], char searchfor[]);

char pattern[] = "ould";	/* pattern to search for */

/* testing strindex; print rightmost index of matching pattern in line */
int main()
{
	char line[MAXLINE];
	int i, found = 0;

	while (getline(line, MAXLINE) > 0)
		if ((i = strindex(line, pattern)) >= 0) {
			printf("%d: %s", i, line);
			found++;
		}
	return found;
}
