#include <stdlib.h>	/* for atof */
#include <stdio.h>

int main()
{
	char a[] = "51541.5e+2e+3";
	char b[] = "51541.e+2";
	float f = -0.0;

	printf("%f\n", atof("."));
	printf("%f\n", atof("-"));
	printf("%f\n", atof("-.5"));
	printf("%f\n", atof(".-5"));
	printf("%f\n", atof(".1"));
	printf("%f\n", atof("1."));
	printf("%f\n", atof(a));
	printf("%f\n", atof(b));
	printf("%f\n", atof("e10"));
	printf("%f\n", atof(".5e+2"));
	printf("%f\n", atof(".e+2"));
	printf("%f\n", atof("-e+2"));
	printf("%f\n", atof("-.e+2"));
	printf("%f\n", atof(".-e+2"));
	printf("%f\n", atof("1234e"));
	printf("%f\n", atof("1234e-.5"));
	printf("%f\n", atof("1234e.5"));
	printf("%f\n", f*1);
	return 0;
}
