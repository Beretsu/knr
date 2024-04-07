/*
 * Exercise 1-17. Write a program to print all input lines that are longer than
 * 80 characters.
*/

#include <stdio.h>

#define MINLEN 80+1		/* minimum output line length */
#define MAXLINE MINLEN+1	/* maximum input line length +1 for '\0' */

int getline(char line[], int maxline);
void copy(char to[], char from[]);

int main()
{
	int c;
	int len;		/* current line length */
	char line[MAXLINE];	/* current input line */

	while ((len = getline(line, MAXLINE)) > 0) {
		if (len >= MINLEN) {
			printf("%s", line);
			if (line[len-1] != '\n') {	/* truncated line */
				while ((c=getchar())!=EOF && c!='\n')
					putchar(c);
				if (c == '\n')
					putchar(c);
			}
		}
	}
	return 0;
}

/* getline: read a line into s, return length */
int getline(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c == '\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}
