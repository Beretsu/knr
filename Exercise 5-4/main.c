#include <stdio.h>
#define MAXLINE 1000

int strend(char *, char *);

int main()
{
	char s[MAXLINE] = "this is a test";
	char *t = "like the other\n";

	printf("%d\n", strend(s, t));
	return 0;
}
