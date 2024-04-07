/*
 * Exercise 2-1. Write a program to determine the ranges of char, short, int,
 * and long variables, both signed and unsigned, by printing appropriate values
 * from standard headers and by direct computation. Harder if you compute them:
 * determine the ranges of the various floating-point types. 
*/

#include <stdio.h>
#include <limits.h>
#include <float.h>

float flt_base_maxexp(void);
float flt_maxmantissa(void);
double dbl_base_maxexp(void);
double dbl_maxmantissa(void);
long double ldbl_base_maxexp(void);
long double ldbl_maxmantissa(void);

/*
 * Prints a table with the limits of basic data types. It Assumes a two's
 * complement machine and floating point numbers with subnormal numbers.
 * It's meaningless to print all digits of a float number. This prints digits
 * upto what fits in the table.
*/
int main()
{
	unsigned char uc_max;		/* unsigned char max */
	unsigned short us_max;		/* unsigned short max */
	unsigned int ui_max;		/* unsigned int max */
	unsigned long ul_max;		/* unsigned long max */
	long v;				/* throwaway var; for cast to long */
	float fx, fy;			/* fx*fy == float max */
	double dx, dy;			/* dx*dy == double max */
	long double ldx, ldy;		/* ldx*ldy == long double max */

	v = -1;
	uc_max = us_max = ui_max = ul_max = v;
	fx = flt_base_maxexp();
	fy = flt_maxmantissa();
	dx = dbl_base_maxexp();
	dy = dbl_maxmantissa();
	ldx = ldbl_base_maxexp();
	ldy = ldbl_maxmantissa();
	printf("\t\t-------------------standard headers--------------------\n");
	printf("signed    char:	[%30d, %30d]\n"  , SCHAR_MIN, SCHAR_MAX);
	printf("unsigned  char:	[%30u, %30u]\n"  , 0        , UCHAR_MAX);
	printf("signed   short:	[%30d, %30d]\n"  , SHRT_MIN , SHRT_MAX);
	printf("unsigned short:	[%30u, %30u]\n"  , 0        , USHRT_MAX);
	printf("signed     int: [%30d, %30d]\n"  , INT_MIN  , INT_MAX);
	printf("unsigned   int: [%30u, %30u]\n"  , 0        , UINT_MAX);
	printf("signed    long: [%30ld, %30ld]\n", LONG_MIN , LONG_MAX);
	printf("unsigned  long:	[%30lu, %30lu]\n", 0        , ULONG_MAX);
	printf("       float: +-[%.25g, %.25g]\n", FLT_MIN  , FLT_MAX);
	printf("      double: +-[%.24g, %.24g]\n", DBL_MIN  , DBL_MAX);
	printf(" long double: +-[%.23Lg, %.23Lg]", LDBL_MIN , LDBL_MAX);
	printf("\n\n");
	printf("\t\t-------------------direct computation------------------\n");
	printf("signed    char:	[%30d, %30d]\n"  , -(uc_max/2 + 1), uc_max/2);
	printf("unsigned  char:	[%30u, %30u]\n"  , 0              , uc_max);
	printf("signed   short:	[%30d, %30d]\n"  , -(us_max/2 + 1), us_max/2);
	printf("unsigned short:	[%30u, %30u]\n"  , 0              , us_max);
	printf("signed     int:	[%30d, %30d]\n"  , -(v=ui_max/2+1), ui_max/2);
	printf("unsigned   int:	[%30u, %30u]\n"  , 0              , ui_max);
	printf("signed    long:	[%30ld, %30ld]\n", -(v=ul_max/2+1), ul_max/2);
	printf("unsigned  long: [%30lu, %30lu]\n", 0              , ul_max);
	printf("       float: +-[%.25g, %.25g]\n", 1/(fx/2)       , fx*fy);
	printf("      double: +-[%.24g, %.24g]\n", 1/(dx/2)       , dx*dy);
	printf(" long double: +-[%.23Lg, %.23Lg]", 1/(ldx/2)      , ldx*ldy);
	putchar('\n');
	return 0;
}

/* flt_base_maxexp: 2 to the power of max exponent for a float */
float flt_base_maxexp(void)
{
	float f1, f2;

	f1 = 1;
	while (f1 != f1*2) {		/* runs FLT_MAX_EXP times */
		f2 = f1;
		f1 = f1*2;
	}
	return f2;
}

/* flt_maxmantissa: max mantissa for a float */
float flt_maxmantissa(void)
{
	float f1, f2;

	f1 = 1;
	while (f1 != f1/2 + 1) {	/* runs FLT_MANT_DIG times */
		f2 = f1;
		f1 = f1/2 + 1;
	}
	return f2;
}

/* dbl_base_maxexp: 2 to the power of max exponent for a double */
double dbl_base_maxexp(void)
{
	double f1, f2;

	f1 = 1;
	while (f1 != f1*2) {		/* runs DBL_MAX_EXP times */
		f2 = f1;
		f1 = f1*2;
	}
	return f2;
}

/* dbl_maxmantissa: max mantissa for a double */
double dbl_maxmantissa(void)
{
	double f1, f2;

	f1 = 1;
	while (f1 != f1/2 + 1) {	/* runs DBL_MANT_DIG times */
		f2 = f1;
		f1 = f1/2 + 1;
	}
	return f2;
}

/* ldbl_base_maxexp: 2 to the power of max exponent for a long double */
long double ldbl_base_maxexp(void)
{
	long double f1, f2;

	f1 = 1;
	while (f1 != f1*2) {		/* runs LDBL_MAX_EXP times */
		f2 = f1;
		f1 = f1*2;
	}
	return f2;
}

/* ldbl_maxmantissa: max mantissa for a long double */
long double ldbl_maxmantissa(void)
{
	long double f1, f2;

	f1 = 1;
	while (f1 != f1/2 + 1) {	/* runs LDBL_MANT_DIG times */
		f2 = f1;
		f1 = f1/2 + 1;
	}
	return f2;
}
