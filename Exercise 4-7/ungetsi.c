/*
 * Exercise 4-7. Write a routine ungets(s) that will push back an entire string
 * onto the input. Should ungets know about buf and bufp, or should it just use
 * ungetch?
*/

/*
 * Using ungetch allows for greater modularity and hides complexity, but that
 * comes at a cost. In this case we wouldn't be able to know if s fits into buf
 * until its full, and then we may have to undo the process.
 * One solution could be to have a function or to use ungetch inside a loop for
 * when you need to unget s as much as possible even if it doesn't fit, and use
 * ungets for when its critical to store s entirely.
 * But this book has disregarded bounds checking in previous exercises, that
 * lead me to belive that this is hinting at using ungetch for ungets. And so
 * that what I'll do. This relegates ungets for when s is guaranteed to fit
 * into buf.
*/

#include <stdio.h>
#include <string.h>

void ungets(char s[]);
int getch(void);
void ungetch(int c);

int main()
{
	char s[] = "this is a test\n";
	int len = strlen(s);

	ungets(s);
	while (len-- > 0)
		putchar(getch());
	return 0;
}

void ungets(char s[])
{
	int i;

	for (i = strlen(s)-1; i >= 0; --i)
		ungetch(s[i]);
}

#define BUFSIZE 100

char buf[BUFSIZE];      /* buffer for ungetch */
int bufp = 0;           /* next free position in buf */

int getch(void)         /* get a (possibly pushed-back) character */
{
        return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)     /* push character back on input */
{
        if (bufp >= BUFSIZE)
                printf("ungetch: to many characters\n");
        else
                buf[bufp++] = c;
}
