#include <stdio.h>
#include <stdlib.h>

/* time save versions: table lookup, idea taken from Gregory Pietsch */
int tisalnum(int c)
{
	return map[(unsigned char)c] & ALNUM;
}

int tisalpha(int c)
{
	return map[(unsigned char)c] & ALPHA;
}

int tiscntrl(int c)
{
	return map[(unsigned char)c] & CNTRL;
}

int tisdigit(int c)
{
	return map[(unsigned char)c] & DIGIT;
}

int tisgraph(int c)
{
	return map[(unsigned char)c] & GRAPH;
}

int tislower(int c)
{
	return map[(unsigned char)c] & LOWER;
}

int tisprint(int c)
{
	return map[(unsigned char)c] & PRINT;
}

int tispunct(int c)
{
	return map[(unsigned char)c] & PUNCT;
}

int tisspace(int c)
{
	return map[(unsigned char)c] & SPACE;
}

int tisupper(int c)
{
	return map[(unsigned char)c] & UPPER;
}

int tisxdigit(int c)
{
	return map[(unsigned char)c] & XDIGIT;
}

/* save space versions. Blatant copy of codybartfast.
 * quoting him:
 * 	Worst case this could result in a switch, 6 integer comparisions and 5
 * logic operations.
 * I'm not sure how this affects performance but it's probably not good that
 * there are perhaps 12 execution routes through the code.
 * While this will take a trivial amount of (modern) processor time, it's easy
 * to imagine this approach being relatively slow compared to a table lookup. 
*/

int sisupper(int c)
{
	switch((unsigned char)'A') {
	case 0x41:	/* ASCII */
		return c >= 'A' && c <= 'Z';
	case 0xC1:	/* EBCDIC */
		return (('A' <= c && c <= 'I') || ('J' <= c && c <= 'R') ||
				('S' <= c && c <= 'Z'));
	default:
		fprintf(stderr, "error: Can't guess encoding. 'A'=0x%X\n",
				'A');
		exit(1);
	}
}
