#include <stdio.h>

int main()
{
	int day_of_year(int, int, int);
	void month_day(int, int, int *, int *);
	int day = 31, month = 12 ,year = 1992, yday;

	yday = day_of_year(year, month, day);
	printf("%d\n", yday);
	month_day(year, yday, &month, &day);
	printf("%d/%d/%d\n",day, month, year);
	return 0;
}
