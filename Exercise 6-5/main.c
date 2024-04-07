/*
 * Exercise 6-5. Write a function undef that will remove a name and definition
 * from the table maintained by lookup and install.
*/

struct nlist {	/* table entry */
	struct nlist *next;	/* next entry in chain */
	char *name;		/* defined name */
	char *defn;		/* replacement text */
};

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
	struct nlist *install(char *, char *);
	void undef(char *);
	void tprint(void);

	install("one", "uno");
	install("two", "dos");
	printf("table before undef\n");
	tprint();
	putchar('\n');
	undef("two");
	printf("table after undef\n");
	tprint();
	return 0;
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

struct nlist *lookup(char *s);
char *strdup(char *);

/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if ((np = lookup(name)) == NULL) {	/* not fount */
		np = (struct nlist *) malloc(sizeof(*np));
		if (np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
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

/* undef: undefine s */
void undef(char *s)
{
	struct nlist *np, *prevp = NULL;
	unsigned hashval = hash(s);

	for (np = hashtab[hashval]; np != NULL; np = np->next) {
		if (strcmp(s, np->name) == 0) {
			if (prevp == NULL)	/* first element */
				hashtab[hashval] = np->next;
			else	/* middle or end element */
				prevp->next = np->next;
			free((void *) np->name);
			free((void *) np->defn);
			free((void *) np);
			break;
		}
		prevp = np;
	}
}

/* tprint: print hashtab */
void tprint(void)
{
	int i;
	struct nlist *np;

	for (i = 0; i < HASHSIZE; i++)
		for (np = hashtab[i]; np != NULL; np = np->next)
			printf("%s:\t%s\n", np->name, np->defn);
}

char *strdup(char *s)	/* make a duplicate of s */
{
	char *p;

	p = (char *) malloc(strlen(s)+1);	/* +1 for '\0' */
	if (p != NULL)
		strcpy(p, s);
	return p;
}
