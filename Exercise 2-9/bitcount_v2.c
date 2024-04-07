/*
 * Exercise 2-9. In a two's complement number system, x &= (x-1) deletes the
 * righmost 1-bit in x. Explain why. Use this observation to write a faster
 * version of bitcount.
*/

/*
 * Think of binary substraction on paper: going from right to left, 0-bits
 * borrow values from the most proximate 1-bit if necesary, and like when
 * subtracting decimals on paper, it substract the lender 1, in binary it turns
 * it to 0; all bits up to the borrower are inverted, but when &ing, all that
 * matters are the 1's turned to 0's, in this case only the lender is turned to
 * 0. The result is the removal of the rightmost 1-bit.
 *
 * new version of bitcount: big O(# of 1-bits)
 * old version of bitcount: big O(# of digits from rightmost bit up to leftmost
 * 1-bit)
*/

#include <stdio.h>

int intbits;			/* size of int in bits */

unsigned getbits(unsigned x, int p, int n);
int bitcount(unsigned x);
void bitcount_print(unsigned x);
void printbinary(unsigned x);
int int_bits(void);

/* testing bitcount */
int main()
{
	int x;

	intbits = int_bits();	/* size of int in bits */
	printf("----------------bitcount(x)-----------------\n");
	for (x = 0x1111; x <= 0xffff; x = x + 0x1111)
		bitcount_print(x);
	return 0;
}

/* getbits: get n bits from position p */
unsigned getbits(unsigned x, int p, int n)
{
        return (x >> (p+1-n)) & ~(~0 << n);
}

/* bitcount: count 1 bits in x; new version */
int bitcount(unsigned x)
{
	int b;

	for (b = 0; x != 0; x &= (x-1))
		++b;
	return b;
}

/* bitcount_print: call bitcount and print its values and result */
void bitcount_print(unsigned x)
{
	printf("%2d : ", bitcount(x));
	printbinary(x);
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
