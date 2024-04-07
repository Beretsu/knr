#include <stdio.h>

int getint(int *);
int getch(void);

int main()
{
	int n, r;

	while ((r = getint(&n)) != EOF) {
		if (r != 0)
			printf("\n%d\n", n);
		else
			putchar(getch());
	}
	return 0;
}
