/*
 * Exercise 3-2. Write a function escape(s,t) that converts characters like
 * newline and tab into visible escape sequences like \n and \t as it copies
 * the string t to s. Use a switch. Write a function for the other direction as
 * well, converting escape sequences into the real characters.
*/

#include <stdio.h>
#define MAXLINE 1000		/* maximum input line */

int getline(char s[], int lim);
void escape(char s[], char t[]);
void unescape(char s[], char t[]);

int main()
{
	int len;
	char s[MAXLINE*2];	/* protect against buffer overflow */
	char t[MAXLINE];

	while ((len = getline(t, MAXLINE)) > 0) {
		escape(s, t);
		printf("%s\n", s);
		unescape(t, s);
		printf("%s", t);
	}
	return 0;
}

/* getline: read a line into s, return length */
int getline(char s[], int lim)
{
	int c, i;

	for (i=0; i<lim-1 && (c = getchar())!='\n' && c!=EOF; ++i)
		s[i] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

/* escape: converts characters to visible escape sequences */
void escape(char s[], char t[])
{
	int i, j;

	j = 0;
	for (i = 0; t[i] != '\0'; ++i) {
		switch (t[i]) {
		case '\t':
			s[j++] = '\\';
			s[j++] = 't';
			break;
		case '\b':
			s[j++] = '\\';
			s[j++] = 'b';
			break;
		case '\n':
			s[j++] = '\\';
			s[j++] = 'n';
			break;
		case '\\':
			s[j++] = '\\';
			s[j++] = '\\';
			break;
		default:
			s[j++] = t[i];
			break;
		}
	}
	s[j] = '\0';
}

/* unescape: inverse of escape */
void unescape(char s[], char t[])
{
	int i, j;

	j = 0;
	for (i = 0; t[i] != '\0'; ++i) {
		if (t[i] == '\\')
			switch (t[++i]) {
			case 't':
				s[j++] = '\t';
				break;
			case 'b':
				s[j++] = '\b';
				break;
			case 'n':
				s[j++] = '\n';
				break;
			default:		/* no odd \ are expected */
				s[j++] = '\\';
				break;
			}
		else 
			s[j++] = t[i];
	}
	s[j] = '\0';
}
