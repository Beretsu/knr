/*
 * Exercise 1-19. Write a function reverse(s) that reverses the character
 * string s. Use it to write a program that reverses its input a line at a
 * time.
*/

#include <stdio.h>
#define MAXLINE 1000	/* maximum input line */
#define IN 1
#define OUT 0

int getline(char line[], int maxline);
void reverse(char s[]);

/* print input lines in reverse; do to memory limitations, at most MAXLINE-1
 * chars are printed for any given line */
int main()
{
	int i;
	int state;		/* IN of OUT of a long line */
	int len;		/* current input line length */
	char line[MAXLINE];	/* current input line */

	while ((len = getline(line, MAXLINE)) > 0 || state == IN) {
		if (len>=MAXLINE-1 && line[len-1]!='\n') {	/* long line */
			state = IN;
		} else {
			reverse(line);
			printf("%s", line);
			if (state == IN) {	/* remains from previous line */
				for (i=MAXLINE-2; line[i]!='\0'; --i)
					putchar(line[i]);
				state = OUT;
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

/* reverse: reverse string s in place */
void reverse(char s[])
{
	int c, i, j;

	i = 0;
	for (j = 0; s[j] != '\0'; ++j)	/* get length of s */
		;
	--j;				/* exclude '\0' */
	while (i < j) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
		++i;
		--j;
	}
}
