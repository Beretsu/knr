/*
 * Exercise 6-2. Write a program that reads a C program and prints in
 * alphabetical order each group of variable names that are identical in the
 * first 6 characters, but different somewhere thereafter. Don't count words
 * within strings and comments. Make 6 a parameter that can be set from the
 * command line.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "stream.h"
#define MAXWORD 100
#define NKEYS (sizeof keywords / sizeof(char *))

enum { ERROR, OK };

char *keywords[] = {
        "auto",         "break",        "case",         "char",
        "const",        "continue",     "default",      "do",
        "double",       "else",         "enum",         "extern",
        "float",        "for",          "goto",         "if",
        "int",          "long",         "register",     "return",
        "short",        "signed",       "sizeof",       "static",
        "struct",       "switch",       "typedef",      "union",
        "unsigned",     "void",         "volatile",     "while"
};

struct gnode {	/* tree node for group of variable names */
	char *id;		/* common first n chars of the names */
	int size;		/* number of member names */
	struct wnode *wtree;	/* tree of variable names */
	struct gnode *left;	/* left child */
	struct gnode *right;	/* right child */
};

struct wnode {	/* tree node for variable name */
	char *word;
	struct wnode *left;
	struct wnode *right;
};

struct gnode *addgroup(struct gnode *, char *, int);
struct wnode *addword(struct wnode *, char *, int *);
void gprint(struct gnode *);
void wprint(struct wnode *);

int binsearch(char *, char **, int);
int status = OK;

/* group variable names */
int main(int argc, char *argv[]) {
	int n;
	char word[MAXWORD];
	struct gnode *root;

	n = 6;
	if (argc > 1 && (n = atoi(argv[1])) <= 0) {
		printf("usage: %s n, where n > 0\n", *argv);
		return 1;
	}
	root = NULL;
	while (status!=ERROR && getword(&strm_code, word, MAXWORD)!=EOF)
		if ((asalpha(word[0]))
		&& binsearch(word, keywords, NKEYS) < 0)
			root = addgroup(root, word, n);
	gprint(root);
	if (status == ERROR)
		printf("error: insufficient memory\n");
	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, char *tab[], int n)
{
        int cond;
        int low, high, mid;

        low = 0;
        high = n - 1;
        while (low <= high) {
                mid = (low+high) / 2;
                if ((cond = strcmp(word, tab[mid])) < 0)
                        high = mid - 1;
                else if (cond > 0)
                        low = mid + 1;
                else
                        return mid;
        }
        return -1;
}

#include <stdlib.h>

/* galloc: make a gnode */
struct gnode *galloc(void)
{
	return (struct gnode *) malloc(sizeof(struct gnode));
}

/* wnode: make a wnode */
struct wnode *walloc(void)
{
	return (struct wnode *) malloc(sizeof(struct wnode));
}

char *strdup(char *);
char *strndup(char *, int);

/* addgroup: add a group node, at or below p */
struct gnode *addgroup(struct gnode *p, char *w, int n)
{
	int cond;
	
	if (p == NULL) {	/* a new group has arrived */
		if ((p = galloc()) == NULL) {	/* make a new node */
			status = ERROR;
			return NULL;
		}
		if ((p->id = strndup(w, n)) == NULL) {
			status = ERROR;
			free((void *) p);
			return NULL;
		}
		p->size = 0;
		if ((p->wtree = addword(NULL, w, &p->size)) == NULL) {
			free((void *) p->id);
			free((void *) p);
			return NULL;
		}
		p->left = p->right = NULL;
	} else if ((cond = strncmp(w, p->id, n)) == 0) {
		p->wtree = addword(p->wtree, w, &p->size);
	} else if (cond < 0) {	/* less than into left subtree */
		p->left = addgroup(p->left, w, n);
	} else {	/* greater than into right subtree */
		p->right = addgroup(p->right, w, n);
	}
	return p;
}

/* addword: add a node with w, at or below p */
struct wnode *addword(struct wnode *p, char *w, int *psize)
{
	int cond;

	if (p == NULL) {
		if ((p = walloc()) == NULL) {
			status = ERROR;
			return NULL;
		}
		if ((p->word = strdup(w)) == NULL) {
			status = ERROR;
			free((void *) p);
			return NULL;
		}
		p->left = p->right = NULL;
		++*psize;
	} else if ((cond = strcmp(w, p->word)) == 0) {
		;	/* do nothing */
	} else if (cond < 0) {
		p->left = addword(p->left, w, psize);
	} else {
		p->right = addword(p->right, w, psize);
	}
	return p;
}

/* gprint: in-order print of tree p */
void gprint(struct gnode *p)
{
	if (p != NULL) {
		gprint(p->left);
		if (p->size > 1) {
			printf("%s\n", p->id);
			wprint(p->wtree);
		}
		gprint(p->right);
	}
}

/* wprint: in-order print of tree p */
void wprint(struct wnode *p)
{
	if (p != NULL) {
		wprint(p->left);
		printf("\t%s\n", p->word);
		wprint(p->right);
	}
}

char *strdup(char *s)	/* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}

char *strndup(char *s, int n)	/* duplicate n chars of s */
{
	char *p;

	p = (char *) malloc(n+1);		/* +1 for '\0' */
	if (p != NULL)
		strncpy(p, s, n);
	return p;
}
