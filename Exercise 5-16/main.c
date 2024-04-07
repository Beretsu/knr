/*
 * Exercise 5-16. Add the -d ("directory order") option, which makes
 * comparisons only on letters, numbers and blanks. Make sure it works in
 * conjunction with -f.
*/

#include <stdio.h>
#include <string.h>

#define MAXLINES 5000		/* max #lines to be sorted */
char *lineptr[MAXLINES];	/* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int getoptions(int argc, char *argv[]);
int (*getcmp(int))(const char *, const char *);

void qsort(void *lineptr[], int left, int right,
		int (*comp)(void *, void *));
int numcmp(const char *, const char *);
int foldcmp(const char *, const char *);
int dircmp(const char *, const char *);
int fdircmp(const char *, const char *);
int revcmp(const char *, const char *);
int (*basecmp)(const char *, const char *);

/* sort input lines */
int main(int argc, char *argv[])
{
	int nlines;	/* number of input lines read */
	int options;	/* options from command line */

	if ((options = getoptions(argc, argv)) < 0)
		return 1;
	if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
		qsort((void**) lineptr, 0, nlines-1,
			(int (*)(void *,void *)) getcmp(options));
		writelines(lineptr, nlines);
		return 0;
	} else {
		printf("input too big to sort\n");
		return 1;
	}
}

#define ALPHA	00		/* alphabetic order */
#define NUMERIC	01		/* numeric order */
#define REVERSE	(01 << 1)	/* decreasing order */
#define FOLD	(01 << 2)	/* ignore case */
#define DIR	(01 << 3)	/* directory order */

/* getoptions: return options if valid, -1 otherwise */
int getoptions(int argc, char *argv[])
{
	int c, options;

	options = 0;
	while (--argc > 0 && **++argv == '-')
		while (c = *++*argv)
			switch (c) {
			case 'n':
				options |= NUMERIC;
				break;
			case 'r':
				options |= REVERSE;
				break;
			case 'f':
				options |= FOLD;
				break;
			case 'd':
				options |= DIR;
				break;
			default:
				printf("error: Illegal option %c\n", c);
				return -1;
			}
	return options;
}

/* getcmp: get corresponding compare function for the given options */
int (*getcmp(int options))(const char *, const char *)
{
	switch(options & ~REVERSE) {
	case ALPHA:
		basecmp = strcmp;
		break;
	case FOLD:
		basecmp = foldcmp;
		break;
	case DIR:
		basecmp = dircmp;
		break;
	case FOLD | DIR:
		basecmp = fdircmp;
		break;
	default:
		basecmp = numcmp;
		break;
	}
	return (options & REVERSE) ? revcmp : basecmp;
}

#define MAXLEN 1000	/* max length of any input line */
int getline(char *, int);
char *alloc(int);

/* readlines: read input lines */
int readlines(char *lineptr[], int maxlines)
{
	int len, nlines;
	char *p, line[MAXLEN];

	nlines = 0;
	while ((len = getline(line, MAXLEN)) > 0) {
		if (nlines >= maxlines || (p = alloc(len)) == NULL)
			return -1;
		else {
			line[len-1] = '\0';	/* delete newline */
			strcpy(p, line);
			lineptr[nlines++] = p;
		}
	}
	return nlines;
}

/* writelines: write output lines */
void writelines(char *lineptr[], int nlines)
{
	while (nlines-- > 0)
		printf("%s\n", *lineptr++);
}

/* qsort: sort v[left]...v[right] into increasing order */
void qsort(void *v[], int left, int right,
		int (*comp) (void *, void *))
{
	int i, last;
	void swap(void *v[], int, int);

	if (left >= right)	/* do nothing if array contains */
		return;		/* fewer than two elements */
	swap(v, left, (left + right)/2);
	last = left;
	for (i = left+1; i <= right; i++)
		if ((*comp) (v[i], v[left]) < 0)
			swap(v, ++last, i);
	swap(v, left, last);
	qsort(v, left, last-1, comp);
	qsort(v, last+1, right, comp);
}

/* numcmp: compare s1 and s2 numerically */
int numcmp(const char *s1, const char *s2)
{
	double atof(const char *);
	double v1, v2;

	v1 = atof(s1);
	v2 = atof(s2);
	if (v1 < v2)
		return -1;
	else if (v1 > v2)
		return 1;
	return 0;
}

/* foldcmp: compare s1 and s2 ignoring case */
int foldcmp(const char *s1, const char *s2)
{
	for ( ; tolower(*s1) == tolower(*s2); ++s1, ++s2)
		if (*s1 == '\0')
			return 0;
	return tolower(*s1) - tolower(*s2);
}


/* dircmp: compare s1 and s2 only on alphanumeric and blanks */
int dircmp(const char *s1, const char *s2)
{
	char c1, c2;

	do {
		while (!isalnum(*s1) && *s1 != ' ' && *s1 != '\0')
			s1++;
		while (!isalnum(*s2) && *s2 != ' ' && *s2 != '\0')
			s2++;
		c1 = *s1++;
		c2 = *s2++;
		if (c1 == c2 && c1 == '\0')
			return 0;
	} while (c1 == c2);
	return c1 - c2;
}

/* fdircmp: compare s1 and s2 only on alphanumeric and blanks, ignoring case */
int fdircmp(const char *s1, const char *s2)
{
	char c1, c2;

	do {
		while (!isalnum(*s1) && *s1 != ' ' && *s1 != '\0')
			++s1;
		while (!isalnum(*s2) && *s2 != ' ' && *s2 != '\0')
			++s2;
		c1 = tolower(*s1++);
		c2 = tolower(*s2++);
		if (c1 == c2 && c1 == '\0')
			return 0;
	} while (c1 == c2);
	return c1 - c2;
}

/* revcmp: compare s1 and s2 in reverse order */
int revcmp(const char *s1, const char *s2)
{
	return basecmp(s2, s1);
}

/* swap: interchange v[i] and v[j] */
void swap(void *v[], int i, int j)
{
	void *temp;

	temp = v[i];
	v[i] = v[j];
	v[j] = temp;
}
