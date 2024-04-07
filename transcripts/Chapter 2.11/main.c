#include <stdio.h>
#define LEN 21

int main()
{
	int a[LEN] = {1,0,4,5,6,7,8,9,3,2,0,3,4,0,2,8,3,1,5,7,9};
	int n = LEN;
	int i;
	for (i = 0; i < n; ++i)
		printf("%6d%c", a[i], (i%10==9 || i == n-1) ? '\n' : ' ');
	printf("you have %d element%s\n", n, (n == 1) ? "" : "s");
	return 0;
}
