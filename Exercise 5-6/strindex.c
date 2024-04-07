/*
 * Exercise 5-6. Rewrite appropriate programs from earlier chapters and
 * exercises with pointers instead of array indexing. Good possibilities
 * include getline (Chapters 1 and 4), atoi, itoa, and their variants
 * (Chapters 2, 3, and 4), reverse (Chapter 3), and strindex and getop
 * (Chapter 4).
*/

/* strindex: return index of t in s, -1 if none */
int strindex(char *s, char *t)
{
	char *p, *ps, *pt;

	for (p = s; *p; p++) {
		for (ps=p, pt=t; *pt && *pt==*ps; ps++, pt++)
			;
		if (pt > t && *pt == '\0')
			return p - s;
	}
	return -1;
}
