/*
 * Exercise 6-6. Implement a simple version of the #define processor (i.e., no
 * arguments) suitable for use with C programs, based on the routines of this
 * section. You may also find getch and ungetch helpful.
*/

struct nlist {	/* table entry */
	struct nlist *next;	/* next entry in chain */
	int isparent;	/* identify recursive defn */
	char *name;	/* defined name */
	char *defn;	/* replacement text */
};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"
#define MAXWORD 100	/* max size of word */
#define MAXDEFN 1000	/* max size of defn */

struct nlist *install(char *, char *);
struct nlist *lookup(char *);
void getdefn(struct stream *, char *, int);
int replace(char *, struct nlist *);
int define(struct stream *, char *, char *);
int trim(char *, int);

enum { ERROR, OK };
enum { NO, YES };

/* #define processor; no support for macros with arguments */
int main(int argc, char *argv[])
{
	struct nlist *np;
	struct charinfo ci;
	char word[MAXWORD];
	char defn[MAXDEFN];
	int status = OK;
	int undone = NO;	/* in unfinished PREPROC? */

	while ((ci = getci(&strm_mode)).c != EOF) {
		if (ci.mode == CODE && asalpha(ci.c) && !undone) {
			ungetci(&strm_mode, ci);
			getword(&strm_mode, LINEBOUND, word, MAXWORD);
			if (np = lookup(word))
				status |= replace(word, np);
			else
				printf("%s", word);
		} else if (ci.mode == PREPROC && !undone) {
			getword(&strm_ncom, LINEBOUND, word, MAXWORD);
			if (strcmp(word, "define") == 0) {
				status |= define(&strm_ncom, word, defn);
			} else if (asalpha(*word)) {
				printf("#%s", word);
				undone = YES;
				if ((ci=getci(&strm_ncom)).mode != SPACER)
                                        ungetci(&strm_mode, ci);
			} else {	/* finished with '\n' or '\0' */
				printf("#%s", word);
			}
		} else {
			putchar(ci.c);
			if (undone && ci.c=='\n' && ci.mode!=COMMENT)
				undone = NO;
		}
	}
	return status;
}

/* define: parse define from stream and install it */
int define(struct stream *strm, char *word, char *defn)
{
	struct charinfo ci;

	getword(strm, LINEBOUND, word, MAXWORD);
	if (*word == '\n' || *word == '\0') {
	       	printf("error: missing define name\n");
		return ERROR;
	} else if (!asalpha(*word)
	|| !isspace((ci=getci(strm)).c) && ci.c!=EOF) {
		getdefn(strm, defn, MAXDEFN);
		printf("error: invalid define name\n");
		return ERROR;
	}
	if (ci.c != '\n' && ci.c != EOF)
		getdefn(strm, defn, MAXDEFN);
	else
		*defn = '\0';
	return install(word, defn) ? OK : ERROR;
}

/* replace: replace word with defn in np */
int replace(char *word, struct nlist *np)
{
	int status, replace_r(char *, char *);

	if (np->isparent) {
		printf("%s", word);
		return ERROR;
	} else {
		np->isparent = YES;
		status = replace_r(word, np->defn);
		np->isparent = NO;
		return status;
	}
}

/* replace_r: parse s and replace defined names */
int replace_r(char *word, char *s)
{
	static int status = OK;
	static struct nlist *np;
	static struct charinfo ci;
	struct bufinfo bi = { s };
	struct intfun buf_f = { (INTFUN) buf_c, (void *) &bi };
	struct cifun make_f = { (CIFUN) make_ci, (void *) &buf_f };
	struct stream strm_buf = { &make_f };
	struct strminfo mode_nfo = { &strm_buf, CODE };
	struct cifun mode_f = { (CIFUN) mode_ci, (void *) &mode_nfo };
	struct stream strm_bm = { &mode_f };

	while ((ci = getci(&strm_bm)).c != EOF) {	/* read from s */
		if (ci.mode == CODE && asalpha(ci.c)) {
			ungetci(&strm_bm, ci);
			getword(&strm_bm, LINEBOUND, word, MAXWORD);
			if (np = lookup(word))
				status |= replace(word, np);
			else
				printf("%s", word);
		} else {
			putchar(ci.c);
		}
	}
	return status;
}

/* getdefn: read defn from strm into w */
void getdefn(struct stream *strm, char *w, int lim)
{
	int len, getdefn_r(struct stream *, char *, int);
	struct charinfo ci;

	while (isblank((ci=getci(strm)).c))
		;
	ungetci(strm, ci);
	len = getdefn_r(strm, w, lim);
	trim(w, len);
}

/* getdefn_r: continue reading defn as long as line doesn't end with '\\' */
int getdefn_r(struct stream *strm, char *w, int lim)
{
	int len;

	len = getline(strm, w, lim);
	if (len > 1 && w[len-1] == '\n' && w[len-2] == '\\')
		return len-2 + getdefn_r(strm, w+len-2, lim-len+2);
	else if (len > 0 && w[len-1] == '\n')
		w[--len] = '\0';
	return len;
}

#define HASHSIZE 101

static struct nlist *hashtab[HASHSIZE];	/* pointer table */

/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;

	for (hashval = 0; *s != '\0'; s++)
	       hashval = *s + 31 * hashval;	
	return hashval % HASHSIZE;
}

char *strdup(char *);

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {	/* not found */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		np->isparent = NO;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	} else	/* already there */
		free((void *) np->defn);	/* free previous defn */
	if ((np->defn =	strdup(defn)) == NULL)
		return NULL;
	return np;
}

/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;

	for (np = hashtab[hash(s)]; np != NULL; np = np->next)
		if (strcmp(s, np->name) == 0)
			return np;	/* found */
	return NULL;	/* not fount */
}

char *strdup(char *s)	/* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}

/* trim: remove trailing space from s */
int trim(char *s, int n)
{
	char *p;

	if (n == 0)
		return n;
	for (p=s+n-1; p > s && isblank(*p); p--)
		;
	if (isblank(*p))
		*p = '\0';
	else
		*++p = '\0';
	return p - s;
}
