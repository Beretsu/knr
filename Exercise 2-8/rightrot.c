/*
 * Exercise 2-8. Write a function rightrot(x,n) that returns the value of the
 * integer x rotated to the right by n positions.
*/

#include <stdio.h>

int intbits;			/* size of int in bits */

unsigned getbits(unsigned x, int p, int n);
unsigned rightrot(unsigned x, int n);
void rightrot_print(unsigned x, int n);
void printbinary(unsigned x);
int int_bits(void);

/* testing rightrot */
int main()
{
	int x, n = 3;

	intbits = int_bits();	/* size of int in bits */
	printf("-------------r = rightrot(x,%d)-------------\n", n);
	for (x = 0x1111; x <= 0xffff; x = x + 0x1111) {
		rightrot_print(x, n);
		putchar('\n');
	}
	return 0;
}

/* getbits: get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
        return (x >> (p+1-n)) & ~(~0 << n);
}

unsigned rightrot(unsigned x, int n)
{
	if (n == 0 || n == intbits)
		return x;
	return x >> n | x << (intbits-n);
}

/* rightrot_print: call rightrot and print its values and result */
void rightrot_print(unsigned x, int n)
{
	printf("x = ");					/* print x */
	printbinary(x);
	putchar('\n');

	printf("r = ");					/* print r */
	printbinary(rightrot(x, n));
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
