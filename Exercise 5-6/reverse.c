/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

/* reverse: reverse string s in place */
void reverse(char *s)
{
	int c;
	char *e;

	for (e = s; *e; ++e)	/* get to end of s */
		;
	for (--e; s < e; ++s, --e)
		c = *s, *s = *e, *e = c;
}
