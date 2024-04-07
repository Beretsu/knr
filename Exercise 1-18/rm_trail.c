/*
 * Exercise 1-18. Write a program to remove trailing blanks and tabs from each
 * line of input, and to delete entirely blank lines.
*/

#include <stdio.h>
#define MAXLINE 1000	/* maximum input line length */
#define IN 1		/* inside a long line */
#define OUT 0		/* outside a long line */

int getline(char line[], int maxline);
int trim(char line[], int len);

/* print trimed lines; do to memory limitations, lines longer
 * than MAXLINE may not be trimmed completely or at all. */
int main()
{
	int state;		/* IN or OUT of a long line */
	int len;		/* current line length */ 
	char line[MAXLINE];	/* current input line */

	state = OUT;
	while ((len = getline(line, MAXLINE)) > 0) {
		if (len>=MAXLINE-1 && line[len-1]!='\n') {	/* long line */
			printf("%s", line);
			state = IN;
		} else if (trim(line, len) > 0) {
			if (line[0]!='\n' || state==IN)	/* not a blank line */
				printf("%s", line);
			state = OUT;
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

/* trim: remove trailing blanks and tabs from s */
int trim(char s[], int len)
{
	int i;

	if (len==0 || s[len-1]!=' ' && s[len-1]!='\t' && s[len-1]!='\n')
		return len;
	for (i=len-2; i>=0 && (s[i]==' ' || s[i]=='\t'); --i)
		;
	if (s[len-1] == '\n') {
		++i;
		s[i] = '\n';
	}
	++i;
	s[i] = '\0';
	return i;
}
