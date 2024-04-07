/*
 * for (i=0; i < lim-1 && (c=getchar()) != '\n' && c != EOF; ++i)
 * 	s[i] = c;
 * Exercise 2-2. Write a loop equiavelent to the for loop above without using
 * && or ||.
*/

#include <stdio.h>
#define MAXLINE 1000	/* maximum input line length */

int getline(char s[], int lim);

/* copy input to output */
int main()
{
	char line[MAXLINE];	/* current input line */

	while (getline(line, MAXLINE) > 0)
		printf("%s", line);
	return 0;
}

/* getline: read a line into s, return length */
int getline(char s[], int lim)
{
	int c, i;

	for (i = 0; i < lim-1; ++i) {
		if ((c = getchar()) != '\n') {
			if (c != EOF) {
				s[i] = c;
			} else {
				s[i] = '\0';
				return i;
			}
		} else {
			s[i] = c;
			++i;
			s[i] = '\0';
			return i;
		}
	}
	s[i] = '\0';
	return i;
}
