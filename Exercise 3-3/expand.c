/*
 * Exercise 3-3. Write a function expand(s1,s2) that expands shorthand notations
 * like a-z in the string s1 into the equivalent complete list abc...xyz in s2.
 * Allow for letters of either case and digits, and be prepared to handle cases
 * like a-b-c and -a-z. Arrange that a leading or trailing - is taken literally.
*/

#include <stdio.h>
#include <ctype.h>
#define MAXLINE 1000
#define MAXEXPA 27+25*((MAXLINE-4)/2)+MAXLINE%2	/* avoid buff overflow */

void expand(char s1[], char s2[]);
int isrange(int, int);

/* testing expand */
int main()
{
	char s1[MAXLINE] = "a-z-aA-Z-A 0-9a-eI-U-a-b-c -abc- a-a c-a 8-e E-i";
	char s2[MAXEXPA];

	expand(s1, s2);
	printf("%s\n%s\n", s1, s2);
	return 0;
}

/* expand: rudimentary shorthand expansion; ascii only */
void expand(char s1[], char s2[])
{
	int i, j, a, b;

	if ((s2[0] = s1[0]) == '\0')
		return;
	for (i = 1, j = 1; s1[i] != '\0'; ++i)
		if (s1[i]=='-' && isrange(a=s1[i-1], b=s1[i+1])) { /* expand */
			if (a < b)
				while (++a < b)
					s2[j++] = a;
			else if (a > b)
				while (--a > b)
					s2[j++] = a;
		} else {
			s2[j++] = s1[i];
		}
	s2[j] = '\0';
}

/* isrange: return 1 if is valid range for shorthand, 0 otherwise */
int isrange(int a, int b)
{
	if (islower(a)) {
		if (islower(b))
			return 1;
	} else if (isupper(a)) {
		if (isupper(b))
			return 1;
	} else if (isdigit(a)) {
		if (isdigit(b))
			return 1;
	}
	return 0;
}
