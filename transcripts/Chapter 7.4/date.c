#include <stdio.h>
#define MAXLINE 1000
#define MAXMONTH 20

int getline(char *, int);

int main()
{
	char line[MAXLINE];
	int day, month, year;
	char monthname[MAXMONTH];

	while (getline(line, sizeof(line)) > 0)
		if (sscanf(line, "%d %s %d", &day, monthname, &year) == 3)
			printf("valid: %s\n", line);	/* 25 Dec 1988 form */
		else if (sscanf(line, "%d/%d/%d", &month, &day, &year) == 3)
			printf("valid: %s\n", line);	/* mm/dd/yy form */
		else
			printf("invalid: %s\n", line);	/* invalid form */
	return 0;
}

int getline(char *s, int lim)
{
	int c, i;
	for (i = 0; --lim > 0 && (c = getchar()) != EOF && c != '\n'; i++)
		s[i] = c;
	if (c == '\n')
		s[i++] = '\n';
	s[i] = '\0';
	return i;
}
