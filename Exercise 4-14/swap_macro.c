/*
 * Exercise 4-14. Define a macro swap(t,x,y) that interchanges two arguments of
 * type t. (Block structure will help.)
*/

#include <stdio.h>
#define swap(t,x,y) {t temp; temp = x, x = y, y = temp;}

int main()
{
	int x = 5, y = 3;

	printf("%d %d\n", x, y);
	swap(int,x,y)
	printf("%d %d\n", x, y);
	return 0;
}
