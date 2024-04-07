/*
 * Exercise 1-11. How would you test the word count program? What kinds of
 * input are most likely to uncover bugs if there are any?
*/

/*
 * Write a unit test.
 *
 * 0. EOF without preceding newline	(bug: doesn't count last line)
 * 1. integer overflow (to many chars, words or newlines)
 * etc.
*/

#include <stdio.h>

#define IN 1	/* inside a word */
#define OUT 0	/* outside a word */

/* count lines, words, and characters in input */
int main()
{
	int c, nl, nw, nc, state;

	state = OUT;
	nl = nw = nc = 0;
	while ((c = getchar()) != EOF) {
		++nc;
		if (c == '\n')
			++nl;
		if (c == ' ' || c == '\t' || c == '\n') {
			state = OUT;
		} else if (state == OUT) {
			state = IN;
			++nw;
		}
	}
	printf("new lines: %d, words: %d, characters: %d\n", nl, nw, nc);
}
