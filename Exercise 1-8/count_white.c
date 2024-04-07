/*
 * Exercise 1-8. Write a program to count blanks, tabs, and newlines.
*/

#include <stdio.h>

int main()
{
	int c, nb, nt, nl;
	
	nb = 0;
	nt = 0;
	nl = 0;
	while ((c = getchar()) != EOF) {
		if (c == ' ')
			++nb;
		if (c == '\t')
			++nt;
		if (c == '\n')
			++nl;
		/* bug? line not counted if EOF not after \n
		 * are we counting the \n char or the lines? */
	}
	printf("blanks: %d, tabs: %d, newlines: %d\n", nb, nt, nl);
}
