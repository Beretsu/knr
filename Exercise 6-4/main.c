/*
 * Exercise 6-4. Write a program that prints the distinct words in its input
 * sorted into decreasing order of frequency of occurrence. Precede each word
 * by its count.
*/

struct wlist {		/* list of words */
	char *word;	/* points to the text */
	struct wlist *next;	/* next node */
};

struct wtree {		/* tree of words */
	char *word;	/* points to the text */
	int count;	/* number of occurrences */
	struct wtree *left;	/* left child */
	struct wtree *right;	/* right child */
};

struct ctree {		/* tree of counts */
	int count;	/* number of occurences */
	struct wlist *words;	/* list of words */
	struct ctree *left;	/* left child */
	struct ctree *right;	/* righ child */
};

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAXWORD 100

enum { ERROR, OK };

struct wtree *addwtree(struct wtree *, char *);
struct ctree *addctree(struct ctree *, char *, int);
struct wlist *addwlist(struct wlist *, char *);
struct ctree *makectree(struct ctree *, struct wtree *);
void ctreeprint(struct ctree *);
int getword(char *, int);
int status = OK;

int main() {
	struct wtree *wroot;
	struct ctree *croot;
	char word[MAXWORD];

	wroot = NULL;
	while (status!=ERROR && getword(word, MAXWORD)!=EOF)
		if (isalpha(word[0]))
			wroot = addwtree(wroot, word);
	croot = makectree(NULL, wroot);
	ctreeprint(croot);
	if (status == ERROR)
		printf("error: insufficient memory\n");
	return status;
}

struct wtree *wtreealloc(void);
struct ctree *ctreealloc(void);
struct wlist *wlistalloc(void);
char *strdup(char *);

/* addwtree: add a node with w, at or below p; compares alphabetically */
struct wtree *addwtree(struct wtree *p, char *w)
{
	int cond;

	if (p == NULL) {	/* a new word has arrived */
		if ((p = wtreealloc()) == NULL) {	/* make a new node */
			status = ERROR;
			return NULL;
		}
		if ((p->word = strdup(w)) == NULL) {
			status = ERROR;
			free((void *) p);
			return NULL;
		}
		p->count = 1;
		p->left = p->right = NULL;
	} else if ((cond = strcmp(w, p->word)) == 0)
		p->count++;	/* repeated word */
	else if (cond < 0)	/* less than into left subtree */
		p->left = addwtree(p->left, w);
	else		/* greater than into right subtree */
		p->right = addwtree(p->right, w);
	return p;
}

/* addwtree: add a node with w, at or below p; compares by count */
struct ctree *addctree(struct ctree *p, char *w, int n)
{
	if (p == NULL) {
		if ((p = ctreealloc()) == NULL) {
			status = ERROR;
			return NULL;
		}
		p->count = n;
		p->words = addwlist(NULL, w);
		p->left = p->right = NULL;
	} else if (n == p->count) {
		p->words = addwlist(p->words, w);
	} else if (n < p->count) {
		p->left = addctree(p->left, w, n);
	} else {
		p->right = addctree(p->right, w, n);
	}
	return p;
}

/* addwlist: add a node with w before the head of list */
struct wlist *addwlist(struct wlist *head, char *w)
{
	struct wlist *p;

	if ((p = wlistalloc()) == NULL) {
		status = ERROR;
		return head;
	}
	p->word = w;
	p->next = head;
	return p;
}

/* makectree: read wt (decreasing alpha order) and fill ct (in count order) */
struct ctree *makectree(struct ctree *ct, struct wtree *wt)
{
	if (wt != NULL) {
		ct = makectree(ct, wt->right);
		ct = addctree(ct, wt->word, wt->count);
		ct = makectree(ct, wt->left);
		free((void *) wt);
	}
	return ct;
}

/* ctreeprint: in-order print of tree p */
void ctreeprint(struct ctree *p)
{
	struct wlist *q;

	if (p != NULL) {
		ctreeprint(p->right);
		for (q = p->words; q != NULL; q = q->next)
			printf("%4d\t%s\n", p->count, q->word);
		ctreeprint(p->left);
	}
}

/* wtreealloc: make a wnode */
struct wtree *wtreealloc(void)
{
	return (struct wtree *) malloc(sizeof(struct wtree));
}

/* ctreealloc: make a ctree */
struct ctree *ctreealloc(void)
{
	return (struct ctree *) malloc(sizeof(struct ctree));
}

/* wlistalloc: make a wlist */
struct wlist *wlistalloc(void)
{
	return (struct wlist *) malloc(sizeof(struct wlist));
}

char *strdup(char *s)	/* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int c, getch(void);
	void ungetch(int);
	char *w = word;

	while (isspace(c = getch()))
		;
	if (c != EOF)
		*w++ = c;
	if (!isalpha(c)) {
		*w = '\0';
		return c;
	}
	for ( ; --lim > 0; w++)
		if (!isalnum(*w = getch())) {
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}
