/*
 * Rewrite appropriate programs from earlier chapters and exercises with
 * pointers instead of array indexing. Good possibilities include getline
 * (Chapters 1 and 4), atoi, itoa, and their variants (Chapters 2, 3, and 4),
 * reverse (Chapter 3), and strindex and getop (Chapter 4).
*/

#include <stdio.h>

/* getline: get line into s, return length */
int getline(char *s, int lim)
{
	int c;
	char *p = s;
	while (--lim > 0 && (c=getchar())!=EOF && c!='\n')
		*s++ = c;
	if (c == '\n')
		*s++ = c;
	*s = '\0';
	return s - p;
}
