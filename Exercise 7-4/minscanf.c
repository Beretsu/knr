/*
 * Exercise 7-4. Write a private version of scanf analogous to minprintf from
 * the previous section.
*/

#include <stdio.h>
#include <stdarg.h>
#define MAXSTR 1000

int main()
{
	int minscanf(char *, ...);
	char c;
	int n;
	char s[MAXSTR];
	float f;
	if (minscanf("%c/%d/%f", &c, &n, &f) == 3)
		printf("valid: %c %d %f\n", c, n, f);
	else
		printf("1st not valid\n");
	if (minscanf("%s", &s) == 1)
		printf("valid %s\n", s);
	else
		printf("2nd not valid\n");
	return 0;
}

#define MAXFORM 100

/* minscanf: minimal scanf with variable argument list */
int minscanf(char *fmt, ...)
{
	char c;
	va_list ap;	/* points to each unnamed arg in turn */
	char *q;
	char subfmt[] = "%\0\0";
	int assigned = 0;

	va_start(ap, fmt);	/* make ap point to 1st unnamed arg */
	for (p = fmt; *p; p++) {
		if (*p != '%')
			if (scanf("%c", &c) != 1 || c != *p)
				return assigned;
			else 
				continue;
		switch (subfmt[1] = *++p) {
			case 'd': case 'i': case 'o': case 'x':
				if (scanf(subfmt, va_arg(ap, int *)) != 1)
					return assigned;
				break;
			case 'u':
				if (scanf(subfmt, va_arg(ap, unsigned*)) != 1)
					return assigned;
				break;
			case 'c': case 's':
				if (scanf(subfmt, va_arg(ap, char *)) != 1)
					return assigned;
				break;
			case 'e': case 'f': case 'g':
				if (scanf(subfmt, va_arg(ap, float *)) != 1)
					return assigned;
				break;
			case '%':
				if (scanf(subfmt, &c) != 1 || c != '%')
					return assigned;
				break;
			default:
				return assigned;
		}
		++assigned;
	}
	va_end(ap);	/* clean up when done */
	return assigned;
}
