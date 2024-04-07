/*
 * Exercise 3-1. Our binary search makes two tests inside the loop, when one
 * would suffice (at the price of more tests outside). Write a version with only
 * one test inside the loop and measure the difference in run-time.
*/

/*
 * surprisingly this version consistently outperformed the original version
 * testing with the linux comand 'time'.
 *
 * original		modified		difference
 * real	1m9.311s	real	0m59.090s	14.75%
 * user	1m7.988s	user	0m57.858s	14.89%
 * sys	0m0.870s	sys	0m0.836s	3.9%
*/

#include <stdio.h>
#define SIZE (1 << 27)		/* to big for the stack allocation */

int v[SIZE];
int binsearch(int x, int v[], int n);

/* test to compare run-time of binsearch versions */
int main()
{
	int i, n = SIZE;

	for (i = 0; i < n; ++i)
		v[i] = i;
	for (i = 0; i < n; ++i)
		binsearch(i, v, n);
	return 0;
}

/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1]; version 2 */
int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low < high) {
		mid = (low+high) / 2;
		if (x > v[mid])
			low = mid + 1;
		else
			high = mid;
	}
	return (x == v[low]) ? low : -1;
}
