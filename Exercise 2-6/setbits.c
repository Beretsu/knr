/*
 * Exercise 2-6. Write a function setbits(x,p,n,y) that returns x with the n
 * bits that begin at position p set to the rightmost n bits of y, leaving the
 * other bits unchanged.
*/

#include <stdio.h>

int intbits;			/* size of int in bits */

unsigned getbits(unsigned x, int p, int n);
unsigned setbits(unsigned x, int p, int n, unsigned y);
void setbits_print(unsigned x, int p, int n, unsigned y);
void printbinary(unsigned x);
int int_bits(void);

/* testing setbits */
int main()
{
	int x, p = 4, n = 3, y = 0x5555;

	intbits = int_bits();	/* size of int in bits */
	printf("------------r = setbits(x,%d,%d,y)-----------\n", p, n);
	for (x = 0x1111; x <= 0xffff; x = x + 0x1111) {
		setbits_print(x, p, n, y);
		putchar('\n');
	}
	return 0;
}

/* getbits: get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
        return (x >> (p+1-n)) & ~(~0 << n);
}

unsigned setbits(unsigned x, int p, int n, unsigned y)
{
	if (n == 0)
		return x;
	x &= ~(~(~0<<n) << (p+1-n));	/* 0 the bits to replace */
	y = (y & ~(~0<<n)) << (p+1-n);	/* align the desired bits, 0 the rest */
	return x | y;
}

/* setbits_print: call setbits and print its values and result */
void setbits_print(unsigned x, int p, int n, unsigned y)
{
	printf("x = ");				/* print x */
	printbinary(x);
	putchar('\n');

	printf("y = ");				/* print y */
	printbinary(y);
	putchar('\n');

	printf("r = ");				/* print r */
	printbinary(setbits(x ,p, n, y));
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
