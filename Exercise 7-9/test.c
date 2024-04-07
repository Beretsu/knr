#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "isfuncs.h"

/* testing isfuncs */
int main(int argc, char *argv[])
{
	if (argc == 1) {
		fprintf(stderr, "usage: %s arg1 arg2 ...", argv[0]);
		exit(1);
	}
	int i, len;
	char *s;
	while (--argc > 0) {
		len = strlen(s = *++argv);
		printf("lower case: ");
		for (i = 0; i < len; ++i)
			if (tislower(s[i]))
				printf("%c, ", s[i]);

		printf("\nupper case: ");
		for (i = 0; i < len; ++i)
			if (tisupper(s[i]))
				printf("%c, ", s[i]);

		printf("\nupper case 2nd ver:");
		for (i = 0; i < len; ++i)
			if (sisupper(s[i]))
				printf("%c, ", s[i]);
		putchar('\n');
		putchar('\n');
	}
	exit(0);
}
