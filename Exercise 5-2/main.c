#include <stdio.h>

int getfloat(double *f);
int getch(void);

/* testing getfloat; breaks lines into numbers and non-numbers; consumes white
 * space */
int main()
{
	int r;
	double f;

	while ((r = getfloat(&f)) != EOF) {
		if (r != 0)
			printf("\n%f\n", f);
		else
			putchar(getch());
	}
	return 0;
}
