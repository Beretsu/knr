#include <stdio.h>
#define NMEMB 10

int main()
{
	void *mycalloc(unsigned, unsigned), myfree(void *);
	int i;
	
	char *v = (char *) mycalloc(NMEMB, sizeof(char));
	for (i = 0; i < NMEMB; ++i)
		putchar(v[i]);
	putchar('\n');
	for (i = 0; i < NMEMB; i++)
		v[i] = getchar();
	for (i = 0; i < NMEMB; i++)
		putchar(v[i]);
	return 0;
}
