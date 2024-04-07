/*
 * Exercise 5-17. Add a field-handling capability, so sorting may be done on
 * fields within lines, each field sorted according to an independent set of
 * options. (The index for this book was sorted with -df for the index category
 * and -n for the page numbers.)
*/

/*
 * Usage is the same as the unix program "sort": eg. "sort -k3,3nr -k1,1r"
 * and behaves like "sort" with the -b option set globally. This program
 * only handles field indexes, not char offsets: "sort -k3.2,4" is not allowed
 * 
 * when global variables are changed, there is no way to set an option to
 * standard sort. I couldn't find a way to do that in unix sort either.
*/

#include <stdio.h>
#include <string.h>
#include "alloc.h"

#define MAXFIELDS 100		/* max #fields allowed per line */
#define MAXLINES 100		/* max #lines to be sorted */
char *lineptr[MAXLINES];	/* pointers to text lines */

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
int parseargs(char *argv[]);
char ***getfields(char *line);
int (*getcmp(int))(const char *, const char *);

void qsort(void *lineptr[], int left, int right,
		int (*comp)(void *, void *));
int numcmp(const char *, const char *);
int foldcmp(const char *, const char *);
int dircmp(const char *, const char *);
int fdircmp(const char *, const char *);
int revcmp(const char *, const char *);
int (*basecmp)(const char *, const char *);
int compare(char ***s1, char ***s2);

int (**cmps)(const char*, const char*);	/* compare function for each key */
int field1[MAXFIELDS];	/* starting field index of a key */
int field2[MAXFIELDS];	/* ending field index of a key, inclusive */
int keyopt[MAXFIELDS];	/* key specific options */
int globopt;		/* global options */
int nkeys;		/* # of keys entered */

enum { SUCCESS, ERROR };

/* sort input lines */
int main(int argc, char *argv[])
{
	int i, nlines;		/* nlines: # of input lines read */
	char ****linefields;	/* lines/line/OUT,IN/field/char */
	int (*globcmp)(const char *, const char *);	/* global cmp */

	if (parseargs(argv) == ERROR)
		return ERROR;
	if ((nlines = readlines(lineptr, MAXLINES)) < 0) {
		printf("input too big to sort\n");
		return ERROR;
	}
	if (nkeys == 0) {
		qsort((void**) lineptr, 0, nlines-1,
			(int (*)(void *,void *)) getcmp(globopt));
		writelines(lineptr, nlines);
		return SUCCESS;
	}
	if ((linefields = pppalloc(nlines)) == NULL
			|| (cmps = falloc(nkeys)) == NULL) {
		printf("error: not enough memmory\n");
		return ERROR;
	}
	for (i = 0; i < nlines; i++)
		if ((linefields[i] = getfields(lineptr[i])) == NULL) {
			printf("error: not enough memmory\n");
			return ERROR;
		}
	globcmp = getcmp(globopt);
	for (i = 0; i < nkeys; i++)
		cmps[i] = keyopt[i] ? getcmp(keyopt[i]) : globcmp;
	qsort((void**) linefields, 0, nlines-1,
		(int (*)(void *,void *)) compare);
	for (i = 0; i < nlines; i++)	/* write lines */
		printf("%s\n", **linefields[i]);
	return SUCCESS;
}

#define ALPHA	00		/* alphabetic order */
#define NUMERIC	01		/* numeric order */
#define REVERSE	(01 << 1)	/* decreasing order */
#define FOLD	(01 << 2)	/* ignore case */
#define DIR	(01 << 3)	/* directory order */

enum { NO, KEYVAL };	/* type of option argument to expect */
int setkeyval(char *argv[]);

/* parseargs: get keys and options */
int parseargs(char *argv[])
{
	int c, req_arg;	/* req_arg: signal an option arg is expected */
	int *options;

	req_arg = NO;
	while (req_arg != NO || *++argv != NULL && **argv == '-') {
		if (req_arg && *++*argv=='\0' && *++argv==NULL) {
			printf("error: option '%c' requires an argument\n", c);
			return ERROR;
		}
		switch (req_arg) {
		case KEYVAL:
			if (setkeyval(argv) == ERROR)
				return ERROR;
			req_arg = NO;
			options = keyopt + nkeys++;	/* read key options */
			break;
		default:
			options = &globopt;	/* read global options */
			break;
		}
		while (req_arg == NO && (c = *++*argv)) {
			switch (c) {
			case 'n':
				*options |= NUMERIC;
				break;
			case 'r':
				*options |= REVERSE;
				break;
			case 'f':
				*options |= FOLD;
				break;
			case 'd':
				*options |= DIR;
				break;
			case 'k':
				req_arg = KEYVAL;
				break;
			default:
				printf("error: Illegal option %c\n", c);
				return ERROR;
			}
		}
	}
	return SUCCESS;
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

/* setkeyval: get field1 and field2 */
int setkeyval(char *argv[])
{
	int n;
	char *s = *argv;

	for (n = 0; isdigit(*s); s++)
		n = n * 10 + (*s - '0');
	if ((field1[nkeys] = n-1) < 0) {
		printf("error: k must be followed by a num > 0\n");
		return ERROR;
	}
	if (*s == ',') {
		for (n = 0; isdigit(*++s); )
			n = n * 10 + (*s - '0');
		if ((field2[nkeys] = n) <= 0) {
			printf("error: ',' must be followed by a num > 0\n");
			return ERROR;
		}
	} else {
		field2[nkeys] = MAXFIELDS;
	}
	*argv = s-1;
	return SUCCESS;
}

#define OUT 0		/* outside a word */
#define IN 1		/* inside a word */

/* getfields: return pointers to the IN and OUT of a line field */
char ***getfields(char *line)
{
	int i, j;
	char ***linefields;
	int state = OUT;

	if ((linefields = ppalloc(2)) == NULL)			/* point to */
		return NULL;
	if ((linefields[OUT] = palloc(MAXFIELDS)) == NULL)	/* OUT */
		return NULL;
	if ((linefields[IN] = palloc(MAXFIELDS)) == NULL)	/* IN */
		return NULL;
	*linefields[OUT] = line;
	for (i = 0; *line; line++) {
		if (state == OUT && !isspace(*line)) {
			linefields[IN][i++] = line;
			state = IN;
		} else if (state == IN && isspace(*line)) {
			linefields[OUT][i] = line;
			state = OUT;
		}
	}
	if (state == OUT)
		linefields[IN][i++] = line;	/* empty field */
	linefields[OUT][i] = line;
	for (j = 0; j < nkeys; j++) {	/* set rest of fields to empty */
		if (field1[j] > i)
			linefields[IN][field1[j]] = line;
		if (field2[j] > i)
			linefields[OUT][field2[j]] = line;
	}
	return linefields;
}


/* compare: compare on fields. Each with independent options */
int compare(char ***s1, char ***s2)
{
	int i, c1, c2, r;
	char *p1, *p2;

	r = 0;
	for (i = 0; i < nkeys && r == 0; ++i) {
		c1 = *(p1 = s1[OUT][field2[i]]);
		c2 = *(p2 = s2[OUT][field2[i]]);
		*p1 = *p2 = '\0';	/* compare only the IN of a field */
		r = (*cmps[i])(s1[IN][field1[i]], s2[IN][field1[i]]);
		*p1 = c1;		/* restore the line */
		*p2 = c2;
	}
	return r;
}

#define MAXLEN 1000	/* max length of any input line */
int getline(char *, int);

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
