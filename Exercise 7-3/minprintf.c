/*
 * Exercise 7-3. Revise minprintf to handle more of the other facilities of
 * printf.
*/

#include <stdio.h>

int main()
{
	void minprintf(char *, ...);
	minprintf("%-0*.*f\n", 10, 3, 51.5143);
	printf("%-0*.*f\n", 10, 3, 51.5143);
	minprintf("%-+ 0#*.*s\n", 15, 10, "hello world");
	minprintf("%-+ 0#*.*s\n", 15, 10, "hello world");
	return 0;
}

#include <stdarg.h>
#include <ctype.h>
#define MAXFORM 100

/* callpritf: call printf with parameters preset */
#define callprintf(T)							\
	if (field && precision)						\
		printf(subfmt, field, precision, va_arg(ap, T));	\
	else if (field)							\
		printf(subfmt, field, va_arg(ap, T));			\
	else if (precision)						\
		printf(subfmt, precision, va_arg(ap, T));		\
	else								\
		printf(subfmt, va_arg(ap, T))

/* minprintf: minimal printf with variable argument list */
void minprintf(char *fmt, ...)
{
	va_list ap;	/* points to each unnamed arg in turn */
	char *p, *q, subfmt[MAXFORM] = "%";
	int field, precision, isflag(char);

	va_start(ap, fmt);	/* make ap point to 1st unnamed arg */
	for (p = fmt; *p; p++) {
		if (*p != '%') {
			putchar(*p);
			continue;
		}
		field = precision = 0;	/* reset encounter */
		q = subfmt + 1;			/* point after % */
		while (isflag(*++p))
			*q++ = *p;
		if (*p == '*') {	/* field is next argument*/
			*q++ = *p++;
			field = va_arg(ap, int);
		} else {		/* field is in format */
			while (isdigit(*p))
				*q++ = *p++;
		}
		if (*p == '.')	/* separates field from precision */
			*q++ = *p++;
		if (*p == '*') {	/* precision is next argument*/
			*q++ = *p++;
			precision = va_arg(ap, int);
		} else {		/* precision is in format */
			while (isdigit(*p))
				*q++ = *p++;
		}
		if (*p == 'h' || *p == 'l' || *p == 'L') /* length modifier */
			*q++ = *p++;
		*(q+1) = '\0';	/* terminate format before conversion char */
		switch (*q = *p) {
		case 'd': case 'i': case 'c':
			callprintf(int);
			break;
		case 'o': case 'x': case 'X': case 'u':
			callprintf(unsigned);
			break;
		case 'f': case 'e': case 'E': case 'g': case 'G':
			callprintf(double);
			break;
		case 's':
			callprintf(char *);
			break;
		case 'p':
			callprintf(void *);
			break;
		case '%':
			putchar('%');
			break;
		default:
			printf(subfmt);
			break;
		}
	}
	va_end(ap);	/* clean up when done */
}

int isflag(char c)
{
	if (c == '-' || c == '+' || c == ' ' || c == '0' || c == '#')
		return 1;
	return 0;
}
