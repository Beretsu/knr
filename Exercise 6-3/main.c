/*
 * Exercise 6-3. Write a cross-referencer that prints a list of all words in a
 * document, and, for each word, a list of the line numbers on which it occurs.
 * Remove noise words like "the," "and," and so on.
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "stream.h"
#define MAXWORD 100
#define NNOISE (sizeof noisewords / sizeof(char *))

enum { ERROR, OK };

char *noisewords[] = {
	"a",	"about", "all",	 "an",	  "and",  "as",
	"at",	"be",	 "but",	 "by",	  "do",	  "for",
	"from",	"get",	 "go",	 "have",  "he",	  "her",
	"his",	"i",	 "if",	 "in",	  "it",	  "me",
	"my",	"not",	 "of",	 "on",	  "one",  "or",
	"out",	"say",	 "she",	 "so",	  "that", "the",
	"their", "there", "they", "this", "to",	  "up",
	"we",	"what",	 "when", "which", "who",  "will",
	"with",	"would", "you"
};

struct lnode {	/* list of line numbers */
	int lineno;
	struct lnode *next;
};
struct lnode *addlist(struct lnode *, int);
void listprint(struct lnode *);

struct tnode {	/* tree node for variable name */
	char *word;
	struct lnode *head;
	struct lnode *tail;
	struct tnode *left;
	struct tnode *right;
};
struct tnode *addtree(struct tnode *, char *, int);
void treeprint(struct tnode *);

int binsearch(char *, char **, int);
int status = OK;

/* cross referecer; case insensitive */
int main() {
	char word[MAXWORD];
	struct tnode *root;
	struct wordinfo wi = { word };

	root = NULL;
	while (status!=ERROR && getword(&strm_lw, &wi, MAXWORD)!=EOF)
		if (asalpha(wi.word[0])
		&& binsearch(wi.word, noisewords, NNOISE) < 0)
			root = addtree(root, wi.word, wi.lineno);
	treeprint(root);
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

struct lnode *lalloc(void);

/* addlist: add n after the last list lnode p */
struct lnode *addlist(struct lnode *p, int n)
{
	if (p == NULL) {
		if ((p = lalloc()) == NULL) {
			status = ERROR;
			return NULL;
		}
		p->lineno = n;
		p->next = NULL;
	} else if (p->lineno != n) {
		if ((p->next = lalloc()) == NULL) {
			status = ERROR;
			return p;
		}
		p->next->lineno = n;
		p->next->next = NULL;
		return p->next;
	}
	return p;
}

/* listprint: print of list p */
void listprint(struct lnode *p)
{
	if (p != NULL) {
		printf("%d", p->lineno);
		p = p->next;
	}
	while (p != NULL) {
		printf(", %d", p->lineno);
		p = p->next;
	}
}

#include <stdlib.h>

/* lalloc: make a lnode */
struct lnode *lalloc(void)
{
	return (struct lnode *) malloc(sizeof(struct lnode));
}

struct tnode *talloc(void);
char *strdup(char *);

/* addtree: add a node with w, at or below p */
struct tnode *addtree(struct tnode *p, char *w, int lineno)
{
	int cond;

	if (p == NULL) {	/* a new word has arrived */
		if ((p = talloc()) == NULL) {	/* make a new node */
			status = ERROR;
			return NULL;
		}
		if ((p->word = strdup(w)) == NULL) {
			status = ERROR;
			free((void *) p);
			return NULL;
		}
		p->head = p->tail = addlist(NULL, lineno);
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0) {
		p->tail = addlist(p->tail, lineno);
	} else if (cond < 0) {	/* less than into left subtree */
		p->left = addtree(p->left, w, lineno);
	} else {		/* greater than into right subtree */
		p->right = addtree(p->right, w, lineno);
	}
	return p;
}

/* treeprint: in-order print of tree p */
void treeprint(struct tnode *p)
{
	if (p != NULL) {
		treeprint(p->left);
		printf("%-30s", p->word);
		listprint(p->head);
		printf("\n");
		treeprint(p->right);
	}
}

/* tnode: make a tnode */
struct tnode *talloc(void)
{
	return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup(char *s)	/* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}
