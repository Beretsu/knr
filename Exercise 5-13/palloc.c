#define ALLOCSIZE 10000	/* size of available space */

static char *allocbuf[ALLOCSIZE];	/* storage for palloc */
static char **allocp = allocbuf;	/* next free position */

char **palloc(int n)	/* return pointer to n pointers to characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) {	/* it fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

void pafree(char **p)	/* free storage pointed to by p */
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
		allocp = p;
}
