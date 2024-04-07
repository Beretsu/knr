/*
 * Exercise 5-4. Write the function strend(s,t), which returns 1 if the string
 * t occurs at the end of the string s, and zero otherwise.
*/

#include <string.h>

int strend(char *s, char *t)
{
	int len_s = strlen(s);
	int len_t = strlen(t);

	if (len_s >= len_t) {
		s += len_s - len_t;
		if (strcmp(s,t) == 0)
			return 1;
	}
	return 0;
}


