/*
 * Exercise 2-4. Write an alternative version of squeeze(s1,s2) that deletes
 * each character in s1 that matches any character in the string s2.
*/

#include <stdio.h>

void squeeze(char s1[], char s2[]);

/* test squeeze */
int main()
{
	char s1[] = "hola compadre cara de hipupotamo";
	char s2[] = "aeiou";

	printf("%s\n", s1);
	printf("%s\n", s2);
	printf("after squeeze\n");
	squeeze(s1, s2);
	printf("%s\n", s1);
	return 0;
}

/* squeeze: delete each char in s1 that matches any character in s2 */
void squeeze(char s1[], char s2[])
{
	int i, j, k;

	k = 0;
	for (i = 0; s1[i] != '\0'; ++i) {
		for (j = 0; s1[i]!=s2[j] && s2[j]!='\0'; ++j)
			;
		if (s2[j] == '\0')
			s1[k++] = s1[i];
	}
	s1[k] = '\0';
}
