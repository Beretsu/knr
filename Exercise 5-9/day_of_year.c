/*
 * Exercise 5-9. Rewrite the routines day_of_year and month_day with
 * pointers instead of indexing.
*/

#include <stdio.h>
#define ERROR -1

static char daytab[][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29 ,31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day)
{
	int i, leap;
	char *p;

	if (month < 1 || month > 12) {
		printf("error: Illegal month\n");
		return ERROR;
	}
	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	p = *(daytab + leap);
	if (day < 1 || day > *(p + month)) {
		printf("error: Illegal day\n");
		return ERROR;
	}
	while (--month)
		day += *++p;
	return day;
}

/* month_day: set month, day from day of year */
void month_day(int year, int yearday, int *pmonth, int *pday)
{
	int i, leap;
	char *p;

	leap = year%4 == 0 && year%100 != 0 || year%400 == 0;
	if (yearday < 1 || yearday > 365+leap) {
		printf("error: Illegal day of year\n");
		*pmonth = *pday = ERROR;
		return;
	}
	p = *(daytab + leap);
	while (yearday > *++p)
		yearday -= *p;
	*pmonth = p - *(daytab + leap);
	*pday = yearday;
}
