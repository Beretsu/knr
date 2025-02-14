/*
 * Exercise 1-5. Modify the temperature conversion program to print the table
 * in reverse order, that is, from 300 degrees to 0.
 */

#include <stdio.h>

/* print Fahrenheit-Celsius table in reverse */
int main()
{
	int fahr;

	printf("Fahrenheit\tCelsius\n");
	printf("----------\t-------\n");
	for (fahr = 300; fahr >= 0; fahr = fahr - 20)
		printf("%10d\t%7.1f\n", fahr, (5.0/9.0)*(fahr-32));
}
