/*
 * Exercise 1-15. rewrite the temperature conversion program of Section 1.2 to
 * use a function for conversion.
*/

#include <stdio.h>

#define LOWER	0	/* lower limit of table */
#define UPPER	300	/* upper limit */
#define STEP	20	/* step size */

float fahrc(float fahr);

/* print Fahrenheit-Celsius table */
int main()
{
	float fahr, celsius;
	
	printf("Fahrenheit\tCelsius\n");
	printf("----------\t-------\n");
	for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP)
		printf("%10.0f\t%7.1f\n", fahr, fahrc(fahr));
	return 0;
}

/* return fahr to Celsius */
float fahrc(float fahr)
{
	return (5.0/9.0) * (fahr-32.0);
}
