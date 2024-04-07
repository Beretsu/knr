/*
 * Exercise 5-5. Write versions of the library functions strncpy, strncat, and
 * strncmp, which operate on at most the first n characters of their argument
 * strings. For example, strncpy(s,t,n) copies at most n characters of t to s.
 * Full descriptions are in Appendix B.
*/

/*
 * strncpy: copy at most n characters of string s to t; return t. Pad with
 * '\0''s if s has fewer than n characters.
*/
char *strncpy(char *s, char *t, int n)
{
	char *pt = t;

	while (n-- > 0 && (*t++ = *s++))
		;
	while (n-- > 0)
		*t++ = '\0';
	*t = '\0';
	return pt;
}

/*
 * strncat: concatenate at most n characters of string s to string t,
 * terminate t with '\0'; return t.
*/
char *strncat(char *s, char *t, int n)
{
	char *pt = t;

	while (*t)
		t++;
	while (n-- > 0 && (*t++ = *s++))
		;
	*t = '\0';
	return pt;
}

/*
 * strncmp: compare at most n characters of string s to string t; return <0
 * if s<t, 0 if s==t, or >0 if s>t.
*/
int strncmp(char *s, char *t, int n)
{
	for ( ; n-- > 0 && *s == *t; s++, t++)
		if (*s == '\0')
			return 0;
	if (n < 0)
		return 0;
	return *s - *t;
}
