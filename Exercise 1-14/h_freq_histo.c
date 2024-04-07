/*
 * Exercise 1-14. Write a program to print a histogram of the frequencies of
 * different characters in its input.
*/

#include <stdio.h>

#define MAXCHAR 256		/* maximum chars in a set */
#define ASCII_A 65		/* ascii value for 'A' */
#define ASCII_FST 32		/* ascii first graphical char */
#define ASCII_LST 126		/* ascii last graphical char */
#define EBCDIC_A 193		/* ebcdic value for 'A' */
#define EBCDIC_FST 64		/* ebcdic first graphical char */
#define EBCDIC_LST 249		/* ebcdic last graphical char */

/* prints histogram horizontally. For ascii and ebcdic only */
int main()
{
	int i, j, start, end;
	int bar[MAXCHAR];	/* stores the frequencies */

        if ('A' == ASCII_A) {                   /* is ascii */
                start = ASCII_FST;
                end = ASCII_LST;
        } else if ('A' == EBCDIC_A) {           /* is ebcdic */
                start = EBCDIC_FST;
                end = EBCDIC_LST;
        } else {                                /* is unknown */
                start = 0;
                end = MAXCHAR-1;
        }
	for (i = 0; i < MAXCHAR; ++i)
		bar[i] = 0;
	while ((i = getchar()) != EOF)
		++bar[i];

	putchar('\n');
	for (i = 0; i < MAXCHAR; ++i) {		/* print table */
		if (i == '\n' || i == '\t' || i >= start && i <= end) {
			if (i == '\n')
				printf("\\n ");
			else if (i == '\t')
				printf("\\t ");
			else
				printf(" %c ", i);
			for (j = 0; j < bar[i]; ++j)
				putchar('#');
			printf("[%d]\n", bar[i]);
		}
	}
}
