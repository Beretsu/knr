/*
 * Exercise 3-1. Our binary search makes two tests inside the loop, when one
 * would suffice (at the price of more tests outside). Write a version with only
 * one test inside the loop and measure the difference in run-time.
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

/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1]; version 1 */
int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	while (low <= high) {
		mid = (low+high) / 2;
		if (x < v[mid])
			high = mid - 1;
		else if (x > v[mid])
			low = mid + 1;
		else	/* found match */
			return mid;
	}
	return -1;	/* no match */
}
