#define ALLOCSIZE 10000	/* size of available space */

static char allocbuf[ALLOCSIZE];	/* storage for alloc */
static char *allocp = allocbuf;		/* next free position */

char *alloc(int n)	/* return pointer to n characters */
{
	if (allocbuf + ALLOCSIZE - allocp >= n) {	/* it fits */
		allocp += n;
		return allocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

void afree(char *p)	/* free storage pointed to by p */
{
	if (p >= allocbuf && p < allocbuf + ALLOCSIZE)
		allocp = p;
}

static char *pallocbuf[ALLOCSIZE];	/* storage for palloc */
static char **pallocp = pallocbuf;	/* next free position */

char **palloc(int n)	/* return pointer to n pointers to characters */
{
	if (pallocbuf + ALLOCSIZE - pallocp >= n) {	/* it fits */
		pallocp += n;
		return pallocp - n;	/* old p */
	} else		/* not enough room */
		return 0;
}

void pafree(char **p)	/* free storage pointed to by p */
{
	if (p >= pallocbuf && p < pallocbuf + ALLOCSIZE)
		pallocp = p;
}

static char **ppallocbuf[ALLOCSIZE];      /* storage for ppalloc */
static char ***ppallocp = ppallocbuf;       /* next free position */

char ***ppalloc(int n)    /* return pointer to n pointers to characters */
{
        if (ppallocbuf + ALLOCSIZE - ppallocp >= n) {     /* it fits */
                ppallocp += n;
                return ppallocp - n;     /* old p */
        } else          /* not enough room */
                return 0;
}

void ppafree(char ***p)   /* free storage pointed to by p */
{
        if (p >= ppallocbuf && p < ppallocbuf + ALLOCSIZE)
                ppallocp = p;
}

static int (*fallocbuf[ALLOCSIZE])(const char *, const char *);
static int (**fallocp)(const char *, const char *) = fallocbuf;

int (**falloc(int n))(const char *, const char *)
{
        if (fallocbuf + ALLOCSIZE - fallocp >= n) {     /* it fits */
                fallocp += n;
                return fallocp - n;     /* old p */
        } else          /* not enough room */
                return 0;
}

void fafree(int (**p)(const char *, const char *))
{
        if (p >= fallocbuf && p < fallocbuf + ALLOCSIZE)
                fallocp = p;
}

static char ***pppallocbuf[ALLOCSIZE];      /* storage for pppalloc */
static char ****pppallocp = pppallocbuf;       /* next free position */

char ****pppalloc(int n)    /* return pointer to n pointers to characters */
{
        if (pppallocbuf + ALLOCSIZE - pppallocp >= n) {     /* it fits */
                pppallocp += n;
                return pppallocp - n;     /* old p */
        } else          /* not enough room */
                return 0;
}

void pppafree(char ****p)   /* free storage pointed to by p */
{
        if (p >= pppallocbuf && p < pppallocbuf + ALLOCSIZE)
                pppallocp = p;
}


