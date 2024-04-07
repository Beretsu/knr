#include <stdio.h>
#define NBYTES 10

int main()
{
	void *mymalloc(unsigned), myfree(void *);
	int i;
	
	char *v = (char *) mymalloc(NBYTES);
	for (i = 0; i < NBYTES; i++)
		v[i] = getchar();
	for (i = 0; i < NBYTES; i++)
		putchar(v[i]);
	return 0;
}
