/*
 * determine which characters are printable from the ascii character set
*/

#include <stdio.h>
#include <ctype.h>

#define ASCII 127

int main()
{
	int i;
	printf("printable characters values:\n");
	for (i = 0; i < ASCII; ++i)
		if (isprint(i))
			printf("%d ", i);
	printf((isprint(i)) ? "%d\n" : "\n", i);
	printf("control characters values:\n");
	for (i = 0; i < ASCII; ++i)
		if (iscntrl(i))
			printf("%d ", i);
	printf((iscntrl(i)) ? "%d\n" : "\n", i);
	return 0;
}
