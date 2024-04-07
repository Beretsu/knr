/*
 * Exercise 1-4. Write a program to print the corresponding Celsius to
 * Fahrenheit table.
*/

#include <stdio.h>

/* print Celsius-Fahrenheit table
 * for celsius = 0, 20, ... , 300 */
int main()
{
	float fahr, celsius;
	int lower, upper, step;

	lower = 0;	/* lower limit of temperature scale */
	upper = 300;	/* upper limit */
	step = 20;	/* step size */

	printf("Celsius\tFahrenheit\n");
	printf("-------\t----------\n");
	celsius = lower;
	while (celsius <= upper) {
		fahr = 9.0/5.0 * celsius + 32.0;
		printf("%7.0f\t%10.1f\n", celsius, fahr);
		celsius = celsius + step;
	}
}
