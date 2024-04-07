#include <stdlib.h>	/* for atof */
#include <stdio.h>

int main()
{
	printf("%f\n", atof("."));
	printf("%f\n", atof("-"));
	printf("%f\n", atof("-.5"));
	printf("%f\n", atof(".-5"));
	printf("%f\n", atof(".1"));
	return 0;
}
