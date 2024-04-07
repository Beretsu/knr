/*
 * Exercise 2-7. Write a function invert(x,p,n) that returns x with the n bits
 * that begin at position p inverted (i.e., 1 changed into 0 and vice versa),
 * leaving the others unchanged.
*/

#include <stdio.h>

int intbits;			/* size of int in bits */

unsigned getbits(unsigned x, int p, int n);
unsigned invert(unsigned x, int p, int n);
void invert_print(unsigned x, int p, int n);
void printbinary(unsigned x);
int int_bits(void);

/* testing invert */
int main()
{
	int x, p = 4, n = 3;

	intbits = int_bits();	/* size of int in bits */
	printf("-------------r = invert(x,%d,%d)-------------\n", p, n);
	for (x = 0x1111; x <= 0xffff; x = x + 0x1111) {
		invert_print(x,p,n);
		putchar('\n');
	}
	return 0;
}

/* getbits: get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
        return (x >> (p+1-n)) & ~(~0 << n);
}

unsigned invert(unsigned x, int p, int n)
{
	if (n == 0)
		return x;
	return x ^ (~(~0<<n) << (p+1-n));
}

/* invert_print: call invert and print its values and result */
void invert_print(unsigned x, int p, int n)
{
	printf("x = ");					/* print x */
	printbinary(x);
	putchar('\n');

	printf("r = ");					/* print r */
	printbinary(invert(x, p, n));
	putchar('\n');
}

/* printbinary: print x in binary form */
void printbinary(unsigned x)
{
	int i;

	for (i = intbits-1; i > 0; --i) {
		putchar(getbits(x, i, 1) + '0');
		if (i%4 == 0)
			putchar(' ');
	}
	putchar(getbits(x, i, 1) + '0');
}

/* int_bits: return size of int in bits */
int int_bits(void)
{
	unsigned nbits, val, overval;

	nbits = val = 1;
	while (val < (overval = val*2)) {	/* up to overflow */
		val = overval;
		++nbits;
	}
	return nbits;
}
