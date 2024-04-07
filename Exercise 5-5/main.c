#include <stdio.h>
#define MAXLINE 1000

char *strncpy(char *, char *, int);
char *strncat(char *, char *, int);
int strncmp(char *, char *, int);

int main()
{
	char *s = "hello world";
	char t[MAXLINE];

	strncpy(s, t, 5);
	printf("%s\n", t);
	s = " I'm here";
	strncat(s, t, 4);
	printf("%s\n", t);
	s = "hello";
	printf("%d\n", strncmp(s, t, 5));
	return 0;
}
