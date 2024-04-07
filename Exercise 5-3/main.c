#include <stdio.h>
#define MAXLINE 1000

void strcat(char *s, char *t);

int main()
{
	char s[MAXLINE] = "this is a test";
	char *t = " like no other\n";
	
	strcat(s, t);
	printf("%s", s);
	return 0;
}
